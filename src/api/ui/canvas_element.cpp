#include "canvas_element.hpp"
#include "terminal_helper.hpp"
#include <iostream>
#include <string>
#include <utility>

CanvasElement::CanvasElement(std::string canvas_element, const ElementSize size) {
	m_canvas_element = std::move(canvas_element);
	m_size = size;
}

CanvasElement::CanvasElement(std::string canvas_element, const int width, const int height) {
	m_canvas_element = std::move(canvas_element);
	m_size = ElementSize{width, height};
}

CanvasElement::CanvasElement(const std::string &normal_string, const char delimiter) {
	m_canvas_element = transform_to_canvas_element(normal_string, delimiter, ' ', m_size);
}

int CanvasElement::get_width() const {
	return m_size.width;
}

int CanvasElement::get_height() const {
	return m_size.height;
}

ElementSize CanvasElement::get_element_size() const {
	return m_size;
}

int CanvasElement::get_total_length() const {
	return m_size.length();
}


const std::string &CanvasElement::get_canvas_element() const {
	return m_canvas_element;
}

std::string &CanvasElement::get_mutable_canvas_element() {
	return m_canvas_element;
}

std::ostream &operator<<(std::ostream &os, const CanvasElement &elem) {
	os << elem.m_canvas_element;
	return os;
}

bool CanvasElement::merge_below_with_other(const CanvasElement &other) {
	if (m_size.width != other.get_width()) {
		return false;
	}
	m_canvas_element += other.get_canvas_element();
	m_size.height += other.get_height();
	return true;
}

bool CanvasElement::merge_above_with_other(const CanvasElement &other) {
	if (m_size.width != other.get_width()) {
		return false;
	}
	m_canvas_element = other.get_canvas_element() + m_canvas_element;
	m_size.height += other.get_height();
	return true;
}

bool CanvasElement::merge_right_with_other(const CanvasElement &other) {
	if (m_size.height != other.get_height()) {
		return false;
	}

	std::string result;
	result.reserve(m_size.length() + other.get_total_length());

	for (int i = 0; i < m_size.height - 1; i++) {
		const std::string_view first(m_canvas_element.data() + i * m_size.width, m_size.width);
		const std::string_view second(other.get_canvas_element().data() + i * other.get_width(), other.get_width());
		result += first;
		result += second;
	}

	m_canvas_element = result;
	m_size.width += other.get_width();
	return true;
}

bool CanvasElement::merge_left_with_other(const CanvasElement &other) {
	if (m_size.height != other.get_height()) {
		return false;
	}

	std::string result;
	result.reserve(m_size.length() + other.get_total_length());

	for (int i = 0; i < m_size.height - 1; i++) {
		const std::string_view first(m_canvas_element.data() + i * m_size.width, m_size.width);
		const std::string_view second(other.get_canvas_element().data() + i * other.get_width(), other.get_width());
		result += second;
		result += first;
	}

	m_canvas_element = result;
	m_size.width += other.get_width();
	return true;
}