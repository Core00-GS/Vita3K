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

#include <lang/state.h>

#include <array>

namespace lang {

static std::array<std::string, static_cast<size_t>(str::_count)> g_strings;

static void init_defaults() {
    auto &s = g_strings;
    auto S = [&](str k, const char *v) { s[static_cast<size_t>(k)] = v; };

    S(str::ok, "OK");
    S(str::cancel, "Cancel");
    S(str::yes, "Yes");
    S(str::no, "No");
    S(str::close, "Close");
    S(str::delete_, "Delete");
    S(str::select, "Select");
    S(str::select_all, "Select All");
    S(str::submit, "Submit");
    S(str::search, "Search");
    S(str::please_wait, "Please wait...");
    S(str::error, "Error");
    S(str::an_error_occurred, "An error occurred.\nError code: {}");
    S(str::could_not_load, "Could not load the file.");
    S(str::could_not_save, "Could not save the file.");
    S(str::file_corrupted, "The file is corrupt.");
    S(str::microphone_disabled, "Enable the microphone.");

    S(str::save_title, "Save");
    S(str::load_title, "Load");
    S(str::saving, "Saving...");
    S(str::loading, "Loading...");
    S(str::saving_complete, "Saving complete.");
    S(str::load_complete, "Loading complete.");
    S(str::deletion_complete, "Deletion complete.");
    S(str::new_saved_data, "New Saved Data");
    S(str::no_saved_data, "There is no saved data.");
    S(str::save_the_data, "Do you want to save the data?");
    S(str::load_saved_data, "Do you want to load this saved data?");
    S(str::delete_saved_data, "Do you want to delete this saved data?");
    S(str::overwrite_saved_data, "Do you want to overwrite this saved data?");
    S(str::cancel_saving, "Do you want to cancel saving?");
    S(str::cancel_loading, "Do you want to cancel loading?");
    S(str::cancel_deleting, "Do you want to cancel deleting?");
    S(str::warning_saving, "Saving...\nDo not power off the system or close the application.");
    S(str::could_not_save_dialog,
        "Could not save the file.\n"
        "There is not enough free space on the memory card. "
        "To save your progress in the application, you must create at least {} of free space.\n\n"
        "To create the free space, press PS button to pause this application, "
        "and then delete other applications or content.");
    S(str::not_free_space,
        "There is not enough free space on the memory card.\n"
        "To continue using the application, you must create at least {} of free space.\n\n"
        "Press the PS button to pause this application, "
        "and then delete other applications or content.");
    S(str::save_details, "Details");
    S(str::save_updated, "Updated");

    S(str::preparing_start_app, "Preparing to start the application...");
    S(str::trophy_earned, "You have earned a trophy!");
    S(str::load_app_failed, "Failed to load \"{}\".");

    S(str::load_app_failed_msg,
        "Failed to load \"{}\"."
        "\nCheck vita3k.log to see console output for details."
        "\n1. Have you installed the firmware?"
        "\n2. Re-dump your own PS Vita app/game and install it on Vita3K."
        "\n3. If you want to install or boot Vitamin, it is not supported.");

    S(str::enter, "Enter");
    S(str::emulation_paused, "Emulation Paused");
    S(str::press_ps_to_continue, "Press PS BUTTON to continue");
    S(str::compiling_shaders, "Please wait, compiling shaders...");
}

struct DefaultInit {
    DefaultInit() { init_defaults(); }
};
static DefaultInit s_init;

const std::string &get(str key) {
    return g_strings[static_cast<size_t>(key)];
}

void set(str key, std::string value) {
    g_strings[static_cast<size_t>(key)] = std::move(value);
}

void reset_defaults() {
    init_defaults();
}

} // namespace lang
