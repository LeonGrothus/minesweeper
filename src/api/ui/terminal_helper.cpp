#include "terminal_helper.hpp"
#include <algorithm>
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

void get_terminal_size(int &width, int &height) {
#if defined(_WIN32)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
	height = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#elif defined(__linux__)
	winsize w{};
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	width = static_cast<int>(w.ws_col);
	height = static_cast<int>(w.ws_row);
#endif
}

std::string repeat_string(const unsigned int k, const std::string &s) {
	if (k == 0)
		return "";
	std::string t;
	for (unsigned int i = 0; i < k; i++)
		t += s;
	return t;
}

std::string transform_to_canvas_element(const std::string &to_canvas_element, const char delimiter,
                                        const char fill_char, ElementSize &size) {
	if (to_canvas_element.empty()) {
		size = ElementSize{0, 0};
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

	const std::string_view last_line(to_canvas_element.data() + prev_pos, to_canvas_element.length() - prev_pos);
	lines.push_back(last_line);
	longest = std::max(longest, last_line.length());

	const int box_height = static_cast<int>(lines.size());
	const int box_width = static_cast<int>(longest);

	std::string result;
	result.reserve(box_width * box_height);

	size = ElementSize(box_width, box_height);
	if (all_same && first_line == false && first_len == last_line.length()) {
		for (const std::string_view &line : lines) {
			result += line;
		}
		return result;
	}

	for (const std::string_view &line : lines) {
		result += line;
		result.append(longest - line.length(), fill_char);
	}

	return result;
}

CanvasElement position_canvas_element(const CanvasElement &element, const Position position,
                                      const ElementSize canvas_size, const char blank_char) {
	const int element_width = element.get_width();
	const int element_height = element.get_height();

	if (element_width > canvas_size.width || element_height > canvas_size.height)
		return CanvasElement("");

	const int repeat_left = (position) & 0b11;
	const int repeat_right = (position >> 2) & 0b11;
	const int repeat_top = (position >> 4) & 0b11;
	const int repeat_bottom = (position >> 6) & 0b11;

	const std::string width_offset = std::string(canvas_size.width, blank_char);
	const std::string half_width_offset = std::string((canvas_size.width - element_width) / 2, blank_char);

	std::string vertical_string_line;
	vertical_string_line.reserve(canvas_size.width);

	std::string vertical_string;
	vertical_string.reserve(canvas_size.width * element_height);

	for (int i = 0; i < element_height; i++) {
		const std::string::size_type current_pos = i * element_width;
		const std::string_view line(element.get_canvas_element().data() + current_pos, element_width);

		vertical_string_line = repeat_string(repeat_left, half_width_offset);
		vertical_string_line += line;
		vertical_string_line += repeat_string(repeat_right, half_width_offset);
		vertical_string_line += std::string(canvas_size.width - vertical_string_line.length(), blank_char);
		vertical_string += vertical_string_line;
	}

	std::string full_string;
	const int height_diff = (canvas_size.height - element_height) / 2;
	const int additional = (canvas_size.height - element_height) % 2;
	full_string.reserve(canvas_size.width * canvas_size.height);

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
	const ElementSize canvas_size = canvas.get_element_size();
	const int canvas_width = canvas_size.width;
	const int canvas_height = canvas_size.height;

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

void render_to_ncurses(const std::string &to_render, const ElementSize size) {
	clear();

	for (int y = 0; y < size.height; y++) {
		const int offset = y * size.width;
		for (int x = 0; x < size.width; x++) {
			mvaddch(y, x, to_render[offset + x]);
		}
	}

	refresh();
}

void show_temporary_message(const std::string &message, const int duration_ms) {
	int width, height;
	get_terminal_size(width, height);

	clear();
	const int y = height / 2;
	int x = (width - static_cast<int>(message.length())) / 2;
	if (x < 0)
		x = 0;
	mvprintw(y, x, "%s", message.c_str());
	refresh();

	napms(duration_ms);
}