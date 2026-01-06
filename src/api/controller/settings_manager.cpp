#include "settings_manager.hpp"

#include <cstring>

#include "api/ui/canvas/terminal_helper.hpp"

SettingsManager::SettingsManager(FileManager file_manager) : m_file_manager(std::move(file_manager)) {
    load_from_file();
}

const Settings &SettingsManager::get_settings() const {
    return m_settings;
}

Settings &SettingsManager::get_settings_mutable() {
    return m_settings;
}

//wanted to serialize the data as JSON, but didn't want to use external libraries
//or implement serialization and deserialization myself, so this has to be enough.

void SettingsManager::save_to_file() const {
    std::string data(sizeof(Settings), ' ');
    std::memcpy(data.data(), &m_settings, sizeof(Settings));
    if (!m_file_manager.write_string_content(data)) {
        show_temporary_message("SAVE NOT WORKING", 1000);
    }
}

void SettingsManager::load_from_file() {
    if (!m_file_manager.file_exists()) {
        return;
    }
    std::string data;
    if (m_file_manager.read_string_content(data) && data.size() == sizeof(Settings)) {
        std::memcpy(&m_settings, data.data(), sizeof(Settings));
    } else {
        //override invalid settings
        save_to_file();
    }
}
