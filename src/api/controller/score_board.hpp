#pragma once

#include "api/helper/file_manager.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>

constexpr int SCORE_BOARD_NUMBER = 5;
constexpr int MAX_NAME_LENGTH = 10;

struct ScoreBoardEntry {
    std::array<char, MAX_NAME_LENGTH + 1> name{};
    int32_t time_in_ms = -1;

    ScoreBoardEntry() = default;

    ScoreBoardEntry(const std::string& str, const int32_t time) : time_in_ms(time) {
        const size_t len = std::min(str.length(), static_cast<size_t>(MAX_NAME_LENGTH));
        std::ranges::copy_n(str.begin(), static_cast<long>(len), name.begin());
        name[len] = '\0';
    }

    std::string get_name() const {
        return name.data();
    }
};

class ScoreBoardManager {
public:
    explicit ScoreBoardManager(std::string path_to_dir);

    std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& entries(int size_key, int difficulty_key);

    void add_entry(int size_key, int difficulty_key, ScoreBoardEntry entry);

    void load_from_files();

    void save_to_files() const;

private:
    std::string m_path_to_dir;

    static std::string make_key(int size_key, int difficulty_key);

    std::unordered_map<std::string, std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>> m_boards;
};
