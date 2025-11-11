#include "canvas_element.hpp"
#include "terminal_helper.hpp"
#include <algorithm>
#include <iostream>
#include <string>

CanvasElement::CanvasElement(std::string canvas_element, int width, int height)
{
    m_canvas_element = canvas_element;
    m_width = width;
    m_height = height;
}

CanvasElement::CanvasElement(const std::string& normal_string, char delimiter)
{
    m_canvas_element = transform_to_canvas_element(normal_string, delimiter, ' ', m_width, m_height);
}

int CanvasElement::get_width() const
{
    return m_width;
}

int CanvasElement::get_height() const
{
    return m_height;
}

const std::string& CanvasElement::get_canvas_element() const
{
    return m_canvas_element;
}

std::string& CanvasElement::get_mutable_canvas_element()
{
    return m_canvas_element;
}

std::ostream& operator<<(std::ostream& os, const CanvasElement& elem) {
    os << elem.m_canvas_element;
    return os;
}