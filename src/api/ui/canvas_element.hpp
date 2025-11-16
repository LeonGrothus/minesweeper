#pragma once

#include <string>

class CanvasElement {
  public:
	explicit CanvasElement(std::string canvas_element, int width, int height);
	explicit CanvasElement(const std::string &normal_string, char delimiter = '\n');

	int get_width() const;
	int get_height() const;

	const std::string &get_canvas_element() const;
	std::string &get_mutable_canvas_element();

	friend std::ostream &operator<<(std::ostream &os, const CanvasElement &elem);

  private:
	int m_width;
	int m_height;

	std::string m_canvas_element;
};