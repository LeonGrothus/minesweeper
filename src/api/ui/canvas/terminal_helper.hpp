#pragma once

#include "canvas_element.hpp"
#include <string>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#include <unistd.h>
#endif

enum Position : uint8_t {
    // bottom, top, right, left
    TOP_LEFT = 0b10001000,
    TOP_CENTER = 0b10000101,
    TOP_RIGHT = 0b10000010,
    MIDDLE_LEFT = 0b01011000,
    MIDDLE_CENTER = 0b01010101,
    MIDDLE_RIGHT = 0b01010010,
    BOTTOM_LEFT = 0b00101000,
    BOTTOM_CENTER = 0b00100101,
    BOTTOM_RIGHT = 0b00100010,
};

Vector2D get_terminal_size();

std::u16string repeat_string(unsigned int k, const std::u16string &s);

CanvasElement position_canvas_element(const CanvasElement &element, Position position, Vector2D canvas_size,
                                      char16_t blank_char);

void position_element_on_canvas(const CanvasElement &element, Position pos, CanvasElement &canvas);

void render_to_ncurses(const CanvasElement &element, Vector2D size);

void render_to_ncurses_buffered(const CanvasElement &element, Vector2D size);

void show_temporary_message(const std::string &message, int duration_ms = 2000);

void show_temporary_message(const std::u16string &message, int duration_ms = 2000);
