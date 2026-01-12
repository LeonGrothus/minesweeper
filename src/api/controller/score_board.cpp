#include "score_board.hpp"

#include <algorithm>
#include <utility>

ScoreBoardManager::ScoreBoardManager(const FileManager& file_manager) {
    (void)file_manager;
}

std::string ScoreBoardManager::make_key(const int size_key, const int difficulty_key) {
    return std::to_string(size_key) + ":" + std::to_string(difficulty_key);
}

std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& ScoreBoardManager::entries(
    const int size_key, const int difficulty_key) {
    return m_boards[make_key(size_key, difficulty_key)];
}

void ScoreBoardManager::add_entry(const int size_key, const int difficulty_key, ScoreBoardEntry entry) {
    const std::string key = make_key(size_key, difficulty_key);
    std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& arr = m_boards[key];

    std::size_t count = 0;
    while (count < arr.size() && arr[count].time_in_ms != -1) {
        ++count;
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
        arr[insert_pos] = std::move(entry);
    }
}
