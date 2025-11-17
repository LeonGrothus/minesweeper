#pragma once

#include <string>

struct ElementSize {
	int width;
	int height;

	auto operator<=>(const ElementSize &a) const = default;

	int length() const {
		return width * height;
	}
};

class CanvasElement {
public:
	explicit CanvasElement(std::string canvas_element, ElementSize size);

	explicit CanvasElement(std::string canvas_element, int width, int height);

	explicit CanvasElement(const std::string &normal_string, char delimiter = '\n');

	int get_width() const;

	int get_height() const;

	ElementSize get_element_size() const;

	int get_total_length() const;

	const std::string &get_canvas_element() const;

	std::string &get_mutable_canvas_element();

	bool merge_below_with_other(const CanvasElement &other);

	bool merge_above_with_other(const CanvasElement &other);

	bool merge_right_with_other(const CanvasElement &other);

	bool merge_left_with_other(const CanvasElement &other);

	friend std::ostream &operator<<(std::ostream &os, const CanvasElement &elem);

private:
	ElementSize m_size{};

	std::string m_canvas_element;
};