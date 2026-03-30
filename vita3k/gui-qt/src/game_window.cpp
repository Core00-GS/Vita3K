// Vita3K emulator project
// Copyright (C) 2026 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <gui-qt/game_window.h>

#include <config/config.h>
#include <config/state.h>
#include <config/version.h>
#include <dialog/state.h>
#include <display/state.h>
#include <ime/state.h>
#include <io/state.h>
#include <renderer/state.h>
#include <touch/state.h>
#include <util/log.h>

#include <QApplication>
#include <QGuiApplication>
#include <QInputMethod>
#include <QInputMethodEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QScreen>
#include <QThread>
#include <QTimer>
#include <QWidget>

#include <algorithm>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#endif

GameWindow::GameWindow(EmuEnvState &emuenv, renderer::Backend backend, QScreen *screen)
    : QWindow(screen)
    , m_emuenv(emuenv)
    , m_backend(backend) {
    if (m_backend == renderer::Backend::OpenGL) {
        setSurfaceType(QSurface::OpenGLSurface);

        m_format.setRenderableType(QSurfaceFormat::OpenGL);
        m_format.setMajorVersion(4);
        m_format.setMinorVersion(3);
        m_format.setProfile(QSurfaceFormat::CoreProfile);
        m_format.setDepthBufferSize(0);
        m_format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        m_format.setSwapInterval(static_cast<int>(emuenv.cfg.current_config.v_sync));
        setFormat(m_format);
    } else {
#ifdef __APPLE__
        setSurfaceType(QSurface::VulkanSurface);
#else
        setSurfaceType(QSurface::RasterSurface);
#endif
    }

    setMinimumSize(QSize(160, 90));
    resize(960, 544);

    const auto &cc = emuenv.cfg.current_config;
    m_title_backend_renderer = cc.backend_renderer;
    m_title_resolution_multiplier = cc.resolution_multiplier;
    m_title_anisotropic_filtering = cc.anisotropic_filtering;
    m_title_screen_filter = cc.screen_filter;
}

GameWindow::~GameWindow() {
    stop_ui_updates();
    if (m_gl_context) {
        m_gl_context->doneCurrent();
        delete m_gl_context;
        m_gl_context = nullptr;
    }
}

bool GameWindow::create_gl_context() {
    if (m_gl_context) {
        LOG_WARN("GL context already created");
        return true;
    }

    m_gl_context = new QOpenGLContext();

    QSurfaceFormat fmt = m_format;
#ifndef NDEBUG
    fmt.setOption(QSurfaceFormat::DebugContext);
#endif

    // Try GL versions 4.6 -> 4.5 -> 4.4
    constexpr int accept_minor_versions[] = { 6, 5, 4 };
    bool created = false;
    for (int minor : accept_minor_versions) {
        fmt.setMajorVersion(4);
        fmt.setMinorVersion(minor);
        m_gl_context->setFormat(fmt);
        if (m_gl_context->create()) {
            created = true;
            LOG_INFO("Created OpenGL {}.{} context", 4, minor);
            break;
        }
    }

    if (!created) {
        LOG_ERROR("Failed to create OpenGL context (need at least ver 4.4)");
        delete m_gl_context;
        m_gl_context = nullptr;
        return false;
    }

    return true;
}

bool GameWindow::make_current() {
    if (!m_gl_context) {
        LOG_ERROR("make_current called with no GL context");
        return false;
    }

    if (m_gl_migration_pending && QThread::currentThread() != m_gl_context->thread()) {
        m_render_thread_id.set_value(QThread::currentThread());
        m_gl_migration_done_future.wait();
        m_gl_migration_pending = false;
    }

    if (!m_gl_context->makeCurrent(this)) {
        LOG_ERROR("makeCurrent failed");
        return false;
    }

    return true;
}

void GameWindow::swap_buffers() {
    if (m_gl_context && isExposed()) {
        m_gl_context->swapBuffers(this);
    }
}

void GameWindow::done_current() {
    if (m_gl_context) {
        m_gl_context->doneCurrent();
        if (QThread::currentThread() != QCoreApplication::instance()->thread()) {
            m_gl_context->moveToThread(QCoreApplication::instance()->thread());
        }
    }
}

void GameWindow::destroy_gl_context() {
    if (m_gl_context) {
        m_gl_context->doneCurrent();
        m_gl_context->aboutToBeDestroyed();
        delete m_gl_context;
        m_gl_context = nullptr;
    }
}

void GameWindow::prepare_gl_for_render_thread() {
    m_render_thread_id = std::promise<QThread *>();
    m_render_thread_id_future = m_render_thread_id.get_future();
    m_gl_migration_done = std::promise<void>();
    m_gl_migration_done_future = m_gl_migration_done.get_future().share();
    m_gl_migration_pending = true;
}

void GameWindow::complete_gl_migration() {
    QThread *render_qthread = m_render_thread_id_future.get();
    m_gl_context->moveToThread(render_qthread);
    m_gl_migration_done.set_value();
}

void GameWindow::start_ui_updates() {
    if (m_ui_timer)
        return;

    m_ui_timer = new QTimer(this);
    m_ui_timer->setInterval(16);
    connect(m_ui_timer, &QTimer::timeout, this, &GameWindow::ui_tick);
    m_ui_timer->start();
}

void GameWindow::stop_ui_updates() {
    if (m_ui_timer) {
        m_ui_timer->stop();
        delete m_ui_timer;
        m_ui_timer = nullptr;
    }
    m_fps_tracking_started = false;
}

void GameWindow::ui_tick() {
    update_window_title();

    const bool dialog_ime_active = (m_emuenv.common_dialog.type == IME_DIALOG
        && m_emuenv.common_dialog.status == SCE_COMMON_DIALOG_STATUS_RUNNING);
    const bool ime_active = m_emuenv.ime.state || dialog_ime_active;
    auto *im = QGuiApplication::inputMethod();

    if (ime_active && !m_ime_was_active) {
        if (m_container) {
            m_container->setAttribute(Qt::WA_InputMethodEnabled, true);
            for (auto *w : m_container->findChildren<QWidget *>())
                w->setAttribute(Qt::WA_InputMethodEnabled, true);
        }
        im->update(Qt::ImEnabled);
    } else if (!ime_active && m_ime_was_active) {
        im->hide();
        if (m_container) {
            m_container->setAttribute(Qt::WA_InputMethodEnabled, false);
            for (auto *w : m_container->findChildren<QWidget *>())
                w->setAttribute(Qt::WA_InputMethodEnabled, false);
        }
        im->update(Qt::ImEnabled);
    }
    m_ime_was_active = ime_active;
}

bool GameWindow::event(QEvent *e) {
    if (e->type() == QEvent::Close) {
        e->ignore();
        return true;
    }
    if (e->type() == QEvent::InputMethodQuery) {
        auto *query = static_cast<QInputMethodQueryEvent *>(e);
        const bool any_ime = m_emuenv.ime.state
            || (m_emuenv.common_dialog.type == IME_DIALOG
                && m_emuenv.common_dialog.status == SCE_COMMON_DIALOG_STATUS_RUNNING);
        if (any_ime) {
            if (query->queries() & Qt::ImEnabled)
                query->setValue(Qt::ImEnabled, true);
            if (query->queries() & Qt::ImHints)
                query->setValue(Qt::ImHints, static_cast<int>(Qt::ImhNone));
            return true;
        }
    }
    return QWindow::event(e);
}

void GameWindow::keyPressEvent(QKeyEvent *e) {
    e->ignore();
}

void GameWindow::keyReleaseEvent(QKeyEvent *e) {
    e->ignore();
}

void GameWindow::update_mouse_position(QMouseEvent *e) {
    auto &touch = m_emuenv.touch;
    const float scale_x = (width() > 0) ? static_cast<float>(client_width_px()) / width() : 1.0f;
    const float scale_y = (height() > 0) ? static_cast<float>(client_height_px()) / height() : 1.0f;
    touch.mouse_x = static_cast<float>(e->position().x()) * scale_x;
    touch.mouse_y = static_cast<float>(e->position().y()) * scale_y;
}

void GameWindow::mousePressEvent(QMouseEvent *e) {
    update_mouse_position(e);
    auto &touch = m_emuenv.touch;
    if (e->button() == Qt::LeftButton)
        touch.mouse_button_left = true;
    if (e->button() == Qt::RightButton)
        touch.mouse_button_right = true;
}

void GameWindow::mouseReleaseEvent(QMouseEvent *e) {
    update_mouse_position(e);
    auto &touch = m_emuenv.touch;
    if (e->button() == Qt::LeftButton)
        touch.mouse_button_left = false;
    if (e->button() == Qt::RightButton)
        touch.mouse_button_right = false;
}

void GameWindow::mouseMoveEvent(QMouseEvent *e) {
    update_mouse_position(e);
}

void GameWindow::focusInEvent(QFocusEvent *) {
    m_emuenv.touch.renderer_focused = true;
}

void GameWindow::focusOutEvent(QFocusEvent *) {
    m_emuenv.touch.renderer_focused = false;
    m_emuenv.touch.mouse_button_left = false;
    m_emuenv.touch.mouse_button_right = false;
}

void GameWindow::set_container(QWidget *container) {
    m_container = container;
}

int GameWindow::client_width_px() const {
#ifdef _WIN32
    RECT rect;
    if (GetClientRect(reinterpret_cast<HWND>(winId()), &rect))
        return rect.right - rect.left;
#endif
    return static_cast<int>(width() * devicePixelRatio());
}

int GameWindow::client_height_px() const {
#ifdef _WIN32
    RECT rect;
    if (GetClientRect(reinterpret_cast<HWND>(winId()), &rect))
        return rect.bottom - rect.top;
#endif
    return static_cast<int>(height() * devicePixelRatio());
}

void GameWindow::toggle_fullscreen() {
    if (!m_container)
        return;

    if (m_container->isFullScreen()) {
        m_container->showNormal();
    } else {
        m_container->showFullScreen();
    }
}

static constexpr uint32_t frames_size = 20;

void GameWindow::update_window_title() {
    if (m_emuenv.frame_count == 0)
        return;

    if (!m_fps_tracking_started) {
        m_fps_tracking_started = true;
        m_last_fps_time = std::chrono::steady_clock::now();
        m_emuenv.frame_count = 0;
        return;
    }

    const auto now = std::chrono::steady_clock::now();
    const uint32_t ms = static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last_fps_time).count());

    if (ms >= 1000) {
        const uint32_t frame_count = static_cast<uint32_t>(m_emuenv.frame_count);
        m_emuenv.fps = (frame_count * 1000 + ms / 2) / ms;
        m_emuenv.ms_per_frame = (ms + frame_count / 2) / frame_count;
        m_last_fps_time = now;
        m_emuenv.frame_count = 0;

        // FPS statistics
        m_emuenv.fps_values[m_emuenv.current_fps_offset] = static_cast<float>(m_emuenv.fps);
        m_emuenv.current_fps_offset = (m_emuenv.current_fps_offset + 1) % frames_size;
        float avg_fps = 0;
        for (uint32_t i = 0; i < frames_size; i++)
            avg_fps += m_emuenv.fps_values[i];
        m_emuenv.avg_fps = static_cast<uint32_t>(avg_fps) / frames_size;
        m_emuenv.min_fps = static_cast<uint32_t>(*std::min_element(m_emuenv.fps_values, std::next(m_emuenv.fps_values, frames_size)));
        m_emuenv.max_fps = static_cast<uint32_t>(*std::max_element(m_emuenv.fps_values, std::next(m_emuenv.fps_values, frames_size)));

        // Push perf data to renderer for the native performance overlay
        if (m_emuenv.renderer) {
            auto &r = *m_emuenv.renderer;
            r.perf_fps = m_emuenv.fps;
            r.perf_avg_fps = m_emuenv.avg_fps;
            r.perf_min_fps = m_emuenv.min_fps;
            r.perf_max_fps = m_emuenv.max_fps;
            r.perf_ms_per_frame = m_emuenv.ms_per_frame;
            std::copy(std::begin(m_emuenv.fps_values), std::end(m_emuenv.fps_values), std::begin(r.perf_fps_values));
            r.perf_fps_values_count = frames_size;
            r.perf_current_fps_offset = m_emuenv.current_fps_offset;
            r.performance_overlay = m_emuenv.cfg.performance_overlay;
            r.performance_overlay_position = m_emuenv.cfg.performance_overlay_position;
            r.performance_overlay_detail = m_emuenv.cfg.performance_overlay_detail;
        }

        const auto af = m_title_anisotropic_filtering > 1
            ? fmt::format(" | AF {}x", m_title_anisotropic_filtering)
            : "";
        const auto x = m_emuenv.display.next_rendered_frame.image_size.x * m_title_resolution_multiplier;
        const auto y = m_emuenv.display.next_rendered_frame.image_size.y * m_title_resolution_multiplier;
        const std::string title = fmt::format("{} | {} ({}) | {} | {} FPS ({} ms) | {}x{}{} | {}",
            window_title,
            m_emuenv.current_app_title, m_emuenv.io.title_id,
            m_title_backend_renderer,
            m_emuenv.fps, m_emuenv.ms_per_frame,
            x, y, af, m_title_screen_filter);

        if (m_container)
            m_container->setWindowTitle(QString::fromStdString(title));
    }
}

void GameWindow::set_paused(bool paused) {
    m_emuenv.drop_inputs = paused;
    m_emuenv.renderer->paused.store(paused, std::memory_order_relaxed);
}
