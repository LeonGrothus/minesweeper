#include "score_board.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <utility>

#include "api/ui/canvas/terminal_helper.hpp"

ScoreBoardManager::ScoreBoardManager(std::string path_to_dir) : m_path_to_dir(std::move(path_to_dir)) {
    load_from_files();
}

std::string ScoreBoardManager::make_key(const int size_key, const int difficulty_key) {
    return std::to_string(size_key) + "_" + std::to_string(difficulty_key);
}

std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& ScoreBoardManager::entries(
    const int size_key, const int difficulty_key) {
    return m_boards[make_key(size_key, difficulty_key)];
}

void ScoreBoardManager::add_entry(const int size_key, const int difficulty_key, const ScoreBoardEntry entry) {
    const std::string key = make_key(size_key, difficulty_key);
    std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& arr = m_boards[key];

    std::size_t count = 0;
    while (count < arr.size() && arr[count].time_in_ms != -1) {
        count++;
    }

    std::size_t insert_pos = count;
    for (std::size_t i = 0; i < count; ++i) {
        if (entry.time_in_ms < arr[i].time_in_ms) {
            insert_pos = i;
            break;
        }
    }

    if (insert_pos < arr.size()) {
        if (count >= arr.size()) {
            std::rotate(arr.begin() + insert_pos, arr.begin() + insert_pos + 1, arr.end());
        } else {
            std::rotate(arr.begin() + insert_pos, arr.begin() + insert_pos, arr.begin() + count + 1);
        }
        arr[insert_pos] = entry;
    }
}

void ScoreBoardManager::load_from_files() {
    std::filesystem::create_directories(m_path_to_dir);
    
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_path_to_dir)) {
        const std::string& key = entry.path().filename().string();

        if (!key.contains('_')) {
            continue;
        }
        FileManager file_manager(entry.path().relative_path().string());

        std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER> score_board_entries;

        std::string data;
        if (file_manager.read_string_content(data) && data.size() == sizeof(std::array<
            ScoreBoardEntry, SCORE_BOARD_NUMBER>)) {
            std::memcpy(&score_board_entries, data.data(), sizeof(std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>));
            m_boards[key] = score_board_entries;
        }
    }
}

void ScoreBoardManager::save_to_files() const {
    for (const auto& [key, value] : m_boards) {
        FileManager file_manager(m_path_to_dir + key);

        std::string data(sizeof(std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>), ' ');
        std::memcpy(data.data(), &value, sizeof(std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>));
        if (!file_manager.write_string_content(data)) {
            show_temporary_message("SAVE NOT WORKING", 1000);
        }
    }
}
