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

enum Position {
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

void get_terminal_size(int &width, int &height);

std::string repeat_string(unsigned int k, const std::string &s);

std::string transform_to_canvas_element(const std::string &to_canvas_element, char delimiter,
                                        char fill_char, ElementSize &size);

CanvasElement position_canvas_element(const CanvasElement &element, Position position, ElementSize canvas_size,
                                      char blank_char);

void position_string_on_canvas(const CanvasElement &element, Position pos, CanvasElement &canvas);

void render_to_ncurses(const std::string &to_render, ElementSize size);

void show_temporary_message(const std::string &message, int duration_ms = 2000);