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

#pragma once

#include <cstdint>
#include <string>

namespace lang {

// Translatable string keys shared across all frontends.
// Qt populates via tr(), Android via getString(), overlay reads via lang::get().
enum class str : uint16_t {
    // Common dialog buttons / labels
    ok,
    cancel,
    yes,
    no,
    close,
    delete_,
    select,
    select_all,
    submit,
    search,
    please_wait,
    error,
    an_error_occurred,
    could_not_load,
    could_not_save,
    file_corrupted,
    microphone_disabled,

    // Save data dialog
    save_title,
    load_title,
    saving,
    loading,
    saving_complete,
    load_complete,
    deletion_complete,
    new_saved_data,
    no_saved_data,
    save_the_data,
    load_saved_data,
    delete_saved_data,
    overwrite_saved_data,
    cancel_saving,
    cancel_loading,
    cancel_deleting,
    warning_saving,
    could_not_save_dialog,
    not_free_space,
    save_details,
    save_updated,

    // Trophy / app loading
    preparing_start_app,
    trophy_earned,
    load_app_failed,

    // Message dialog
    load_app_failed_msg,

    // Overlay UI
    enter,
    emulation_paused,
    press_ps_to_continue,
    compiling_shaders,

    _count
};

// Get a translated string. Returns English default if no translation set.
const std::string &get(str key);

// Set a translated string for a given key.
void set(str key, std::string value);

// Reset all strings to English defaults.
void reset_defaults();

} // namespace lang
