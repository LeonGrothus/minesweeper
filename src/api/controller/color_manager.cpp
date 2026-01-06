
#include "color_manager.hpp"

#include <array>
#include <curses.h>

#include "../ui/canvas/canvas_element.hpp"

static std::array<short, static_cast<size_t>(ColorRole::Count)> g_color_pairs{};

short get_color_for_role(const uint8_t role) {
    if (const size_t idx = role; idx < g_color_pairs.size()) {
        return g_color_pairs[idx];
    }
    return g_color_pairs[static_cast<size_t>(ColorRole::Default)];
}


void init_terminal_colors() {
    if (!has_colors()) {
        return;
    }

    start_color();
    use_default_colors();

    init_pair(1, -1, -1);
    init_pair(2, COLOR_WHITE, -1); //hidden
    init_pair(3, COLOR_RED, -1); //mine
    init_pair(4, COLOR_YELLOW, -1); //flag
    init_pair(5, COLOR_YELLOW, -1); //cursor

    init_pair(6, COLOR_CYAN, -1); //1
    init_pair(7, COLOR_GREEN, -1); //2
    init_pair(8, COLOR_RED, -1); //3
    init_pair(9, COLOR_BLUE, -1); //4
    init_pair(10, COLOR_MAGENTA, -1); //5
    init_pair(11, COLOR_CYAN, -1); //6
    init_pair(12, COLOR_BLACK, -1); //7
    init_pair(13, COLOR_WHITE, -1); //8

    set_terminal_monochrome(1);
    set_terminal_colored();
}

void set_terminal_colored() {
    g_color_pairs[static_cast<size_t>(ColorRole::Default)] = 1;
    g_color_pairs[static_cast<size_t>(ColorRole::Hidden)] = 2;
    g_color_pairs[static_cast<size_t>(ColorRole::Mine)] = 3;
    g_color_pairs[static_cast<size_t>(ColorRole::Flag)] = 4;
    g_color_pairs[static_cast<size_t>(ColorRole::Cursor)] = 5;
    g_color_pairs[static_cast<size_t>(ColorRole::Number1)] = 6;
    g_color_pairs[static_cast<size_t>(ColorRole::Number2)] = 7;
    g_color_pairs[static_cast<size_t>(ColorRole::Number3)] = 8;
    g_color_pairs[static_cast<size_t>(ColorRole::Number4)] = 9;
    g_color_pairs[static_cast<size_t>(ColorRole::Number5)] = 10;
    g_color_pairs[static_cast<size_t>(ColorRole::Number6)] = 11;
    g_color_pairs[static_cast<size_t>(ColorRole::Number7)] = 12;
    g_color_pairs[static_cast<size_t>(ColorRole::Number8)] = 13;
    g_color_pairs[static_cast<size_t>(ColorRole::Text)] = 1;
    g_color_pairs[static_cast<size_t>(ColorRole::Transition)] = 1;
}

void set_terminal_monochrome(const short role) {
    g_color_pairs[static_cast<size_t>(ColorRole::Default)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Hidden)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Mine)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Flag)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Cursor)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Number1)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Number2)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Number3)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Number4)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Number5)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Number6)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Number7)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Number8)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Text)] = role;
    g_color_pairs[static_cast<size_t>(ColorRole::Transition)] = role;
}

std::vector<ColorRole> get_all_colors() {
    return std::vector<ColorRole>{
        ColorRole::Default,
        ColorRole::Mine, //red
        ColorRole::Flag, //yellow
        ColorRole::Number2, //green
        ColorRole::Number1, //cyan
        ColorRole::Number4, //blue
        ColorRole::Number5, //magenta
        ColorRole::Number7, //black
        ColorRole::Hidden //white
    };
}

std::vector<ColorRole> get_all_colors_except_black() {
    return std::vector<ColorRole>{
        ColorRole::Default,
        ColorRole::Mine, //red
        ColorRole::Flag, //yellow
        ColorRole::Number2, //green
        ColorRole::Number1, //cyan
        ColorRole::Number4, //blue
        ColorRole::Number5, //magenta
        ColorRole::Hidden //white
    };
}
