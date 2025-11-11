#include "terminal_helper.hpp"
#include <algorithm>
#include <iostream>
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

void get_terminal_size(int &width, int &height) {
#if defined(_WIN32)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
	height = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#elif defined(__linux__)
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	width = (int)(w.ws_col);
	height = (int)(w.ws_row);
#endif
}

std::string repeat_string(unsigned int k, std::string &s) {
	if (k == 0)
		return "";
	std::string t;
	for (unsigned int i = 0; i < k; i++)
		t += s;
	return t;
}

std::string transform_to_canvas_element(const std::string &to_canvas_element, char delimiter, char fill_char,
										int &box_width, int &box_height) {
	if (to_canvas_element.empty()) {
		box_height = box_width = 0;
		return to_canvas_element;
	}

	std::vector<std::string_view> lines;
	std::string::size_type longest = 0;

	std::string::size_type prev_pos = 0;
	std::string::size_type current_pos;

	bool first_line = true;
	std::string::size_type first_len = 0;
	bool all_same = true;

	while ((current_pos = to_canvas_element.find(delimiter, prev_pos)) != std::string::npos) {
		std::string_view line(to_canvas_element.data() + prev_pos, current_pos - prev_pos);
		lines.push_back(line);
		longest = std::max(longest, line.length());
		prev_pos = current_pos + 1;

		if (first_line) {
			first_len = longest;
			first_line = false;
		} else if (first_len != line.length()) {
			all_same = false;
		}
	}

	std::string_view last_line(to_canvas_element.data() + prev_pos, to_canvas_element.length() - prev_pos);
	lines.push_back(last_line);
	longest = std::max(longest, last_line.length());

	box_height = lines.size();
	box_width = longest;
	if (all_same && first_line == false && first_len == last_line.length()) {
		std::string result;
		result.reserve(box_width * box_height);
		for (const std::string_view &line : lines) {
			result += line;
		}
		return result;
	}

	std::string result;
	result.reserve(box_width * box_height);

	for (const std::string_view &line : lines) {
		result += line;
		result.append(longest - line.length(), fill_char);
	}

	return result;
}

std::string position_canvas_element(const CanvasElement &element, Position position, int canvas_width,
									int canvas_height, char blank_char) {
	int element_width = element.get_width();
	int element_height = element.get_height();

	if (element_width > canvas_width || element_height > canvas_height)
		return "";

	int repeat_left = position & 0b11;
	int repeat_right = (position >> 2) & 0b11;
	int repeat_top = (position >> 4) & 0b11;
	int repeat_bottom = (position >> 6) & 0b11;

	std::string width_offset = std::string(canvas_width, blank_char);
	std::string half_width_offset = std::string((canvas_width - element_width) / 2, blank_char);

	std::string vertical_string_line;
	vertical_string_line.reserve(canvas_width);

	std::string vertical_string;
	vertical_string.reserve(canvas_width * element_height);

	std::string::size_type current_pos;

	for (int i = 0; i < element_height; i++) {
		current_pos = i * element_width;
		std::string_view line(element.get_canvas_element().data() + current_pos, element_width);

		vertical_string_line = repeat_string(repeat_left, half_width_offset);
		vertical_string_line += line;
		vertical_string_line += repeat_string(repeat_right, half_width_offset);
		vertical_string_line += std::string(canvas_width - vertical_string_line.length(), blank_char);
		vertical_string += vertical_string_line;
	}

	std::string full_string;
	int height_diff = (canvas_height - element_height) / 2;
	int additional = (canvas_height - element_height) % 2;
	full_string.reserve(canvas_width * canvas_height);

	bool extra_at_top = (repeat_bottom != 0);

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
	return full_string;
}

void position_string_on_canvas(const CanvasElement &element, Position pos, CanvasElement &canvas) {
	int canvas_width = canvas.get_width();
	int canvas_height = canvas.get_height();

	if (element.get_width() > canvas_width || element.get_height() > canvas_height) {
		return;
	}

	//'\x7F' is non printable delete char
	std::string alpha_canvas = position_canvas_element(element, pos, canvas_width, canvas_height, '\x7F');

	const std::string &element_canvas = element.get_canvas_element();
	std::string &canvas_canvas = canvas.get_mutable_canvas_element();

	for (int i = 0; i < canvas_width * canvas_height; i++) {
		if (alpha_canvas[i] == '\x7F')
			continue;

		canvas_canvas[i] = alpha_canvas[i];
	}
}