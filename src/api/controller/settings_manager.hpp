#pragma once
#include "api/helper/file_manager.hpp"

constexpr static std::string FILE_LOCATION = "mine_config";

struct Settings {
    bool show_milliseconds = false;
    bool use_color = true;
};

class SettingsManager {
public:
    explicit SettingsManager(FileManager file_manager);

    const Settings &get_settings() const;

    Settings &get_settings_mutable();

    void load_from_file();

    void save_to_file() const;

private:
    FileManager m_file_manager;

    Settings m_settings;
};
