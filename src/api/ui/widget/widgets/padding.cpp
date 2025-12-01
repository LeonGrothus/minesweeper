#include "padding.hpp"

Padding::Padding(std::unique_ptr<Widget> child, const int all_sides) : m_child(std::move(child)), m_left(all_sides),
                                                                       m_right(all_sides), m_top(all_sides),
                                                                       m_bottom(all_sides) {
}

Padding::Padding(std::unique_ptr<Widget> child, const int left, const int right, const int top,
                 const int bottom) : m_child(std::move(child)), m_left(left),
                                     m_right(right), m_top(top), m_bottom(bottom) {
}

void Padding::set_border_char(const char border_char) {
    m_border_char = border_char;
}

void Padding::set_fill_char(const char fill_char) {
    m_fill_char = fill_char;
}

bool Padding::is_dirty() const {
    return m_child->is_dirty();
}

const CanvasElement &Padding::build_widget(const Vector2D &size) {
    if (size < get_minimum_size()) {
        return m_child->build_widget(size);
    }
    const Vector2D child_size = size - Vector2D(m_left + m_right, m_top + m_bottom);
    const CanvasElement &child = m_child->build_widget(child_size);

    std::string padding;
    padding.reserve(size.area());

    std::string horizontal_padding;
    horizontal_padding.reserve(child_size.area() + (m_left + m_right) * child.get_height());
    for (int i = 0; i < child.get_height(); i++) {
        if (m_left > 0) {
            horizontal_padding += m_border_char;
            horizontal_padding.append(m_left - 1, m_fill_char);
        }
        std::string_view line(child.get_canvas_element().data() + i * child.get_width(), child.get_width());
        horizontal_padding.append(line);
        if (m_right > 0) {
            horizontal_padding.append(m_right - 1, m_fill_char);
            horizontal_padding += m_border_char;
        }
    }

    if (m_top > 0 || m_bottom > 0) {
        const std::string border_line(size.x, m_border_char);
        std::string fill_line;
        fill_line.reserve(size.x);
        int left_right_padding = 0;
        if (m_left > 0) {
            left_right_padding++;
            fill_line += m_border_char;
        }
        if (m_right > 0) {
            left_right_padding++;
        }
        fill_line.append(size.x - left_right_padding, m_fill_char);
        if (m_right > 0) {
            fill_line += m_border_char;
        }

        if (int top = m_top; top > 0) {
            padding.append(border_line);
            while (--top > 0) {
                padding.append(fill_line);
            }
        }
        padding.append(horizontal_padding);
        if (int bottom = m_bottom; bottom > 0) {
            while (--bottom > 0) {
                padding.append(fill_line);
            }
            padding.append(border_line);
        }
    } else {
        m_cached_canvas = CanvasElement(horizontal_padding, size);
        return m_cached_canvas;
    }
    m_cached_canvas = CanvasElement(padding, size);
    return m_cached_canvas;
}

Vector2D Padding::get_minimum_size() const {
    return m_child->get_minimum_size() + Vector2D(m_left + m_right, m_top + m_bottom);
}

void Padding::keyboard_press(const int key) {
    m_child->keyboard_press(key);
}

void Padding::update(const double delta_time) {
    m_child->update(delta_time);
}
