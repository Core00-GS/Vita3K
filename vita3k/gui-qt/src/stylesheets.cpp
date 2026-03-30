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

#include <gui-qt/stylesheets.h>

namespace gui::stylesheets {

// Default (Bright) built-in stylesheet
const QString default_style_sheet(
    "QToolBar#mw_toolbar { background-color: #F0F0F0; border: none; border-top: 1px solid #cfcfcf; }"
    "QToolBar#mw_toolbar::separator { background-color: rgba(207, 207, 207, 235); width: 0.125em; margin-top: 0.250em; margin-bottom: 0.250em; }"
    "QToolButton:disabled { color: #787878; }"
    "QTableWidget { alternate-background-color: #f2f2f2; background-color: #fff; border: none; }"
    "QTableView::item { border-left: 0.063em solid white; border-right: 0.063em solid white; padding-left: 0.313em; }"
    "QTableView::item:selected { background-color: #148aff; color: #fff; }"
    "QHeaderView::section { padding-left: .5em; padding-right: .5em; padding-top: .4em; padding-bottom: -.1em; border: 0.063em solid #ffffff; }"
    "QHeaderView::section:hover { background: #e3e3e3; }"
    "QDockWidget { background: transparent; color: black; }"
    "QDockWidget[floating=\"true\"] { background: white; }"
    "QDockWidget::title { background: #e3e3e3; border: none; padding-top: 0.2em; padding-left: 0.2em; }"
    "QDockWidget::close-button, QDockWidget::float-button { background-color: #e3e3e3; }"
    "QPlainTextEdit#log_frame { background-color: #ffffff; color: #000000; }"
    "QMenu { color: #000; background-color: #F0F0F0; alternate-background-color: #f2f2f2; }"
    "QMenu::item:selected { background: #90C8F6; }"
    "QMenu::item:disabled { color: #787878; }"
    "QStatusBar { background-color: #F0F0F0; }"
    "QStatusBar::item { border: none; }");

// Default (Dark) built-in stylesheet — Fusion Dark
const QString dark_style_sheet(
    "QWidget { color: #e0e0e0; background-color: #353535; }"
    "QMainWindow { background-color: #353535; }"
    "QDialog { background-color: #353535; }"
    "QMenuBar { background-color: #353535; color: #e0e0e0; }"
    "QMenuBar::item { background: transparent; padding: 4px 8px; }"
    "QMenuBar::item:selected { background: #4a4a4a; }"
    "QMenu { background-color: #353535; color: #e0e0e0; border: 1px solid #555555; }"
    "QMenu::item:selected { background-color: #2a82da; }"
    "QMenu::item:disabled { color: #808080; }"
    "QMenu::separator { height: 1px; background: #555555; margin: 4px 8px; }"
    "QToolBar#mw_toolbar { background-color: #353535; border: none; border-top: 1px solid #555555; }"
    "QToolBar#mw_toolbar::separator { background-color: rgba(100, 100, 100, 235); width: 0.125em; margin-top: 0.250em; margin-bottom: 0.250em; }"
    "QToolButton { color: #e0e0e0; background: transparent; border: 1px solid transparent; padding: 3px; }"
    "QToolButton:hover { background-color: #4a4a4a; border: 1px solid #666666; }"
    "QToolButton:pressed { background-color: #2a82da; }"
    "QToolButton:disabled { color: #808080; }"
    "QPushButton { background-color: #353535; color: #e0e0e0; border: 1px solid #555555; border-radius: 2px; padding: 4px 16px; min-height: 1.2em; }"
    "QPushButton:hover { background-color: #4a4a4a; border-color: #666666; }"
    "QPushButton:pressed { background-color: #2a82da; }"
    "QPushButton:disabled { color: #808080; background-color: #303030; border-color: #444444; }"
    "QPushButton:default { border-color: #2a82da; }"
    "QLineEdit { background-color: #2a2a2a; color: #e0e0e0; border: 1px solid #555555; border-radius: 2px; padding: 2px 4px; selection-background-color: #2a82da; selection-color: #ffffff; }"
    "QLineEdit:focus { border-color: #2a82da; }"
    "QComboBox { background-color: #353535; color: #e0e0e0; border: 1px solid #555555; border-radius: 2px; padding: 2px 6px; }"
    "QComboBox:hover { border-color: #666666; }"
    "QComboBox::drop-down { border: none; width: 20px; }"
    "QComboBox::down-arrow { image: none; border-left: 4px solid transparent; border-right: 4px solid transparent; border-top: 6px solid #e0e0e0; margin-right: 6px; }"
    "QComboBox QAbstractItemView { background-color: #2a2a2a; color: #e0e0e0; border: 1px solid #555555; selection-background-color: #2a82da; selection-color: #ffffff; }"
    "QSpinBox, QDoubleSpinBox { background-color: #2a2a2a; color: #e0e0e0; border: 1px solid #555555; border-radius: 2px; padding: 2px; selection-background-color: #2a82da; }"
    "QCheckBox { color: #e0e0e0; spacing: 6px; }"
    "QCheckBox::indicator { width: 14px; height: 14px; border: 1px solid #555555; border-radius: 2px; background: #2a2a2a; }"
    "QCheckBox::indicator:checked { background: #2a82da; border-color: #2a82da; }"
    "QCheckBox::indicator:hover { border-color: #888888; }"
    "QRadioButton { color: #e0e0e0; spacing: 6px; }"
    "QRadioButton::indicator { width: 14px; height: 14px; border: 1px solid #555555; border-radius: 8px; background: #2a2a2a; }"
    "QRadioButton::indicator:checked { background: #2a82da; border-color: #2a82da; }"
    "QRadioButton::indicator:hover { border-color: #888888; }"
    "QGroupBox { color: #e0e0e0; border: 1px solid #555555; border-radius: 4px; margin-top: 0.5em; padding-top: 0.6em; }"
    "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 4px; color: #e0e0e0; }"
    "QLabel { color: #e0e0e0; background: transparent; }"
    "QTableWidget, QTableView { alternate-background-color: #3a3a3a; background-color: #2a2a2a; border: none; color: #e0e0e0; gridline-color: #444444; }"
    "QTableView::item { border-left: 0.063em solid #2a2a2a; border-right: 0.063em solid #2a2a2a; padding-left: 0.313em; }"
    "QTableView::item:selected { background-color: #2a82da; color: #ffffff; }"
    "QHeaderView::section { background-color: #353535; color: #e0e0e0; padding-left: .5em; padding-right: .5em; padding-top: .4em; padding-bottom: -.1em; border: 0.063em solid #2a2a2a; }"
    "QHeaderView::section:hover { background: #4a4a4a; }"
    "QDockWidget { background: transparent; color: #e0e0e0; }"
    "QDockWidget[floating=\"true\"] { background: #353535; }"
    "QDockWidget::title { background: #353535; border: none; padding-top: 0.2em; padding-left: 0.2em; }"
    "QDockWidget::close-button, QDockWidget::float-button { background-color: #353535; border: none; }"
    "QDockWidget::close-button:hover, QDockWidget::float-button:hover { background-color: #4a4a4a; }"
    "QPlainTextEdit#log_frame { background-color: #2a2a2a; color: #e0e0e0; }"
    "QTabWidget::pane { border: 1px solid #555555; background-color: #353535; }"
    "QTabBar::tab { background-color: #2d2d2d; color: #e0e0e0; padding: 6px 14px; border: 1px solid #555555; border-bottom: none; border-top-left-radius: 3px; border-top-right-radius: 3px; }"
    "QTabBar::tab:selected { background-color: #353535; }"
    "QTabBar::tab:!selected { margin-top: 2px; }"
    "QTabBar::tab:hover:!selected { background-color: #3a3a3a; }"
    "QListWidget { background-color: #2a2a2a; color: #e0e0e0; border: 1px solid #555555; }"
    "QListWidget::item:selected { background-color: #2a82da; color: #ffffff; }"
    "QListWidget::item:hover:!selected { background-color: #3a3a3a; }"
    "QScrollBar:vertical { background: #2a2a2a; width: 14px; margin: 0; }"
    "QScrollBar::handle:vertical { background: #5a5a5a; min-height: 20px; border-radius: 3px; margin: 2px; }"
    "QScrollBar::handle:vertical:hover { background: #6a6a6a; }"
    "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
    "QScrollBar:horizontal { background: #2a2a2a; height: 14px; margin: 0; }"
    "QScrollBar::handle:horizontal { background: #5a5a5a; min-width: 20px; border-radius: 3px; margin: 2px; }"
    "QScrollBar::handle:horizontal:hover { background: #6a6a6a; }"
    "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; }"
    "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }"
    "QSlider::groove:horizontal { background: #555555; height: 4px; border-radius: 2px; }"
    "QSlider::handle:horizontal { background: #c0c0c0; width: 14px; margin: -5px 0; border-radius: 7px; }"
    "QSlider::handle:horizontal:hover { background: #ffffff; }"
    "QSlider::groove:vertical { background: #555555; width: 4px; border-radius: 2px; }"
    "QSlider::handle:vertical { background: #c0c0c0; height: 14px; margin: 0 -5px; border-radius: 7px; }"
    "QSlider::handle:vertical:hover { background: #ffffff; }"
    "QProgressBar { background-color: #2a2a2a; color: #e0e0e0; border: 1px solid #555555; text-align: center; border-radius: 2px; }"
    "QProgressBar::chunk { background-color: #2a82da; border-radius: 2px; }"
    "QSplitter::handle { background-color: #555555; }"
    "QToolTip { background-color: #353535; color: #e0e0e0; border: 1px solid #555555; padding: 2px; }"
    "QPlainTextEdit { background-color: #2a2a2a; color: #e0e0e0; border: 1px solid #555555; }"
    "QTextEdit { background-color: #2a2a2a; color: #e0e0e0; border: 1px solid #555555; }"
    "QTextBrowser { background-color: #2a2a2a; color: #e0e0e0; border: 1px solid #555555; }"
    "QStatusBar { background-color: #353535; color: #e0e0e0; }"
    "QStatusBar::item { border: none; }"
    "QDialogButtonBox QPushButton { min-width: 80px; }");

} // namespace gui::stylesheets
