#pragma once

#include <string>

struct Vector2D {
	int x;
	int y;

	auto operator<=>(const Vector2D &a) const = default;

	Vector2D operator +(const Vector2D &other) const {
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D operator -(const Vector2D &other) const {
		return Vector2D(x - other.x, y - other.y);
	}

	Vector2D operator %(const Vector2D &other) const {
		return Vector2D(x % other.x, y % other.y);
	}

	int area() const {
		return x * y;
	}
};

class CanvasElement {
public:
	explicit CanvasElement(std::string canvas_element, Vector2D size);

	explicit CanvasElement(std::string canvas_element, int width, int height);

	explicit CanvasElement(const std::string &normal_string, char delimiter = '\n');

	static CanvasElement empty(Vector2D size, char empty_char);

	int get_width() const;

	int get_height() const;

	Vector2D get_element_size() const;

	int get_total_length() const;

	const std::string &get_canvas_element() const;

	std::string &get_mutable_canvas_element();

	CanvasElement fill_to_size(Vector2D size, char fill_char) const;

	bool merge_below_with_other(const CanvasElement &other);

	bool merge_above_with_other(const CanvasElement &other);

	bool merge_right_with_other(const CanvasElement &other);

	bool merge_left_with_other(const CanvasElement &other);

	friend std::ostream &operator<<(std::ostream &os, const CanvasElement &elem);

private:
	static CanvasElement transform_to_canvas_element(const std::string &to_canvas_element, char delimiter,
	                                                 char fill_char);

	Vector2D m_size{};

	std::string m_canvas_element;
};