#pragma once

#include "api/helper/file_manager.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>

constexpr int SCORE_BOARD_NUMBER = 5;
constexpr int MAX_NAME_LENGTH = 10;

struct ScoreBoardEntry {
    std::string name;
    int32_t time_in_ms = -1;
};

class ScoreBoardManager {
public:
    explicit ScoreBoardManager(const FileManager& file_manager);

    std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& entries(int size_key, int difficulty_key);

    void add_entry(int size_key, int difficulty_key, ScoreBoardEntry entry);

    // void load_from_file();

    // void save_to_file() const;

private:
    static std::string make_key(int size_key, int difficulty_key);

    std::unordered_map<std::string, std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>> m_boards;
};
