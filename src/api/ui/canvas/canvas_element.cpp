#include "terminal_helper.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <string_view>
#include <ncurses.h>

CanvasElement::CanvasElement(std::string canvas_element, const Vector2D size) {
	m_canvas_element = std::move(canvas_element);
	m_size = size;
}

CanvasElement::CanvasElement(std::string canvas_element, const int width, const int height) {
	m_canvas_element = std::move(canvas_element);
	m_size = Vector2D{width, height};
}

CanvasElement::CanvasElement(const std::string &normal_string, const char delimiter) {
	const CanvasElement canvas = transform_to_canvas_element(normal_string, delimiter, ' ');
	m_canvas_element = canvas.get_canvas_element();
	m_size = canvas.m_size;
}

CanvasElement CanvasElement::empty(const Vector2D size, const char empty_char) {
	return CanvasElement(std::string(size.area(), empty_char), size);
}

int CanvasElement::get_width() const {
	return m_size.x;
}

int CanvasElement::get_height() const {
	return m_size.y;
}

Vector2D CanvasElement::get_element_size() const {
	return m_size;
}

int CanvasElement::get_total_length() const {
	return m_size.area();
}


const std::string &CanvasElement::get_canvas_element() const {
	return m_canvas_element;
}

std::string &CanvasElement::get_mutable_canvas_element() {
	return m_canvas_element;
}

CanvasElement CanvasElement::fill_to_size(const Vector2D size, const char fill_char) const {
	if (m_size > size) {
		return CanvasElement(m_canvas_element, m_size);
	}

	std::string new_canvas_element;
	new_canvas_element.reserve(size.area());

	for (int i = 0; i < m_size.y; i++) {
		const std::string_view line(m_canvas_element.data() + i * m_size.x, m_size.x);
		new_canvas_element += line;
		new_canvas_element.append(size.x - m_size.x, fill_char);
	}

	new_canvas_element.append((size.y - m_size.y) * size.x, fill_char);

	return CanvasElement(std::move(new_canvas_element), size);
}

std::ostream &operator<<(std::ostream &os, const CanvasElement &elem) {
	os << elem.m_canvas_element;
	return os;
}

bool CanvasElement::merge_below_with_other(const CanvasElement &other) {
	if (m_size.x != other.get_width()) {
		return false;
	}
	m_canvas_element += other.get_canvas_element();
	m_size.y += other.get_height();
	return true;
}

bool CanvasElement::merge_above_with_other(const CanvasElement &other) {
	if (m_size.x != other.get_width()) {
		return false;
	}
	m_canvas_element = other.get_canvas_element() + m_canvas_element;
	m_size.y += other.get_height();
	return true;
}

bool CanvasElement::merge_right_with_other(const CanvasElement &other) {
	if (m_size.y != other.get_height()) {
		return false;
	}

	std::string result;
	result.reserve((m_size.x + other.get_width()) * m_size.y);

	for (int i = 0; i < m_size.y; i++) {
		const std::string_view first(m_canvas_element.data() + i * m_size.x, m_size.x);
		const std::string_view second(other.get_canvas_element().data() + i * other.get_width(), other.get_width());
		result += first;
		result += second;
	}

	m_canvas_element = result;
	m_size.x += other.get_width();
	return true;
}

bool CanvasElement::merge_left_with_other(const CanvasElement &other) {
	if (m_size.y != other.get_height()) {
		return false;
	}

	std::string result;
	result.reserve((m_size.x + other.get_width()) * m_size.y);

	for (int i = 0; i < m_size.y; i++) {
		const std::string_view first(m_canvas_element.data() + i * m_size.x, m_size.x);
		const std::string_view second(other.get_canvas_element().data() + i * other.get_width(), other.get_width());
		result += second;
		result += first;
	}

	m_canvas_element = result;
	m_size.x += other.get_width();
	return true;
}

CanvasElement CanvasElement::transform_to_canvas_element(const std::string &to_canvas_element, const char delimiter,
                                                         const char fill_char) {
	if (to_canvas_element.empty()) {
		constexpr Vector2D size(0, 0);
		return CanvasElement(to_canvas_element, size);
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

	const Vector2D size(box_width, box_height);
	if (all_same && first_line == false && first_len == last_line.length()) {
		for (const std::string_view &line : lines) {
			result += line;
		}
		return CanvasElement(result, size);
	}

	for (const std::string_view &line : lines) {
		result += line;
		result.append(longest - line.length(), fill_char);
	}

	return CanvasElement(result, size);
}