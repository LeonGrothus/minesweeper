#include "terminal_helper.hpp"
#include <algorithm>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <string_view>
#include <vector>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#include <unistd.h>
#endif

Vector2D get_terminal_size() {

	if (stdscr != nullptr) {
		return Vector2D{COLS, LINES};
	}
#if defined(_WIN32)
	CONSOLE_SCREEN_BUFFER_INFO console_info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info);
	width = (int)(console_info.srWindow.Right - console_info.srWindow.Left + 1);
	height = (int)(console_info.srWindow.Bottom - console_info.srWindow.Top + 1);
	return Vector2D{width, height};
#elif defined(__linux__)
	winsize w{};
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return Vector2D{(w.ws_col - 40), w.ws_row};
#endif
}

std::string repeat_string(const unsigned int k, const std::string &s) {
	if (k == 0)
		return "";
	std::string t;
	for (int i = 0; i < k; i++)
		t += s;
	return t;
}

CanvasElement position_canvas_element(const CanvasElement &element, const Position position,
                                      const Vector2D canvas_size, const char blank_char) {
	const int element_width = element.get_width();
	const int element_height = element.get_height();

	if (element.get_element_size() > canvas_size)
		return CanvasElement::empty(canvas_size, ' ');
	if (element.get_element_size() == canvas_size)
		return element;

	const int repeat_left = (position) & 0b11;
	const int repeat_right = (position >> 2) & 0b11;
	const int repeat_top = (position >> 4) & 0b11;
	const int repeat_bottom = (position >> 6) & 0b11;

	const std::string width_offset = std::string(canvas_size.x, blank_char);
	const std::string half_width_offset = std::string((canvas_size.x - element_width) / 2, blank_char);

	std::string vertical_string_line;
	vertical_string_line.reserve(canvas_size.x);

	std::string vertical_string;
	vertical_string.reserve(canvas_size.x * element_height);

	for (int i = 0; i < element_height; i++) {
		const std::string::size_type current_pos = i * element_width;
		const std::string_view line(element.get_canvas_element().data() + current_pos, element_width);

		vertical_string_line = repeat_string(repeat_left, half_width_offset);
		vertical_string_line += line;
		vertical_string_line += repeat_string(repeat_right, half_width_offset);
		vertical_string_line += std::string(canvas_size.x - vertical_string_line.length(), blank_char);
		vertical_string += vertical_string_line;
	}

	std::string full_string;
	const int height_diff = (canvas_size.y - element_height) / 2;
	const int additional = (canvas_size.y - element_height) % 2;
	full_string.reserve(canvas_size.x * canvas_size.y);

	const bool extra_at_top = (repeat_bottom != 0);

	for (int i = 0; i < repeat_top * height_diff; i++) {
		full_string += width_offset;
	}
	if (additional && extra_at_top) {
		full_string += width_offset;
	}
	full_string += vertical_string;
	for (int i = 0; i < repeat_bottom * height_diff; i++) {
		full_string += width_offset;
	}
	if (additional && !extra_at_top) {
		full_string += width_offset;
	}
	return CanvasElement(full_string, canvas_size);
}

void position_string_on_canvas(const CanvasElement &element, const Position pos, CanvasElement &canvas) {
	const Vector2D canvas_size = canvas.get_element_size();
	const int canvas_width = canvas_size.x;
	const int canvas_height = canvas_size.y;

	if (element.get_width() > canvas_width || element.get_height() > canvas_height) {
		return;
	}

	//'\x7F' is non printable delete char
	const std::string alpha_canvas = position_canvas_element(element, pos, canvas_size, '\x7F').get_canvas_element();
	std::string &canvas_canvas = canvas.get_mutable_canvas_element();

	for (int i = 0; i < canvas_width * canvas_height; i++) {
		if (alpha_canvas[i] == '\x7F')
			continue;

		canvas_canvas[i] = alpha_canvas[i];
	}
}

void render_to_ncurses(const std::string &to_render, const Vector2D size) {
	clear();

	// if (to_render.size() < size.x * size.y) {
	// 	std::cerr << "string to short" << std::endl;
	// 	return;
	// }

	for (int y = 0; y < size.y; y++) {
		const int offset = y * size.x;
		mvaddnstr(y, 0, to_render.c_str() + offset, size.x);
	}

	refresh();
}

std::string last_frame;

void render_to_ncurses_buffered(const std::string &to_render, const Vector2D size) {
	if (last_frame.size() != to_render.size()) {
		last_frame.assign(to_render.size(), '\x7F');
	}

	for (int i = 0; i < to_render.size(); i++) {
		if (last_frame[i] == to_render[i]) {
			continue;
		}
		const int x = i % size.x;
		const int y = i / size.x;

		mvaddch(y, x, to_render[i]);
	}
	last_frame = to_render;

	wnoutrefresh(stdscr);
	doupdate();
}

void show_temporary_message(const std::string &message, const int duration_ms) {
	auto [width, height] = get_terminal_size();

	clear();
	const int y = height / 2;
	int x = (width - static_cast<int>(message.length())) / 2;
	if (x < 0)
		x = 0;
	mvprintw(y, x, "%s", message.c_str());
	refresh();

	napms(duration_ms);
}