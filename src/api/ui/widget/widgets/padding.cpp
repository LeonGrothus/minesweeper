#include "padding.hpp"

#include <vector>

Padding::Padding(std::shared_ptr<Widget> child, const int all_sides) : m_child(std::move(child)), m_left(all_sides),
                                                                       m_right(all_sides), m_top(all_sides),
                                                                       m_bottom(all_sides) {
}

Padding::Padding(std::shared_ptr<Widget> child, const int left, const int right, const int top,
                 const int bottom) : m_child(std::move(child)), m_left(left),
                                     m_right(right), m_top(top), m_bottom(bottom) {
}

void Padding::set_fill_char(const char16_t fill_char) {
    m_fill_char = fill_char;
}

CanvasElement Padding::build_canvas_element(const Vector2D &size) {
    if (size < get_minimum_size()) {
        return m_child->build_widget(size);
    }

    const Vector2D child_size = size - Vector2D(m_left + m_right, m_top + m_bottom);
    const CanvasElement &child = m_child->build_widget(child_size);

    std::u16string padding_canvas;
    std::vector<uint8_t> padding_roles;
    padding_canvas.reserve(size.area());
    padding_roles.reserve(size.area());

    const std::u16string top_bottom_padding(size.x, m_fill_char);
    const std::vector<uint8_t> default_line(static_cast<size_t>(size.x), static_cast<uint8_t>(ColorRole::Default));
    for (int i = 0; i < m_top; ++i) {
        padding_canvas.append(top_bottom_padding);
        padding_roles.insert(padding_roles.end(), default_line.begin(), default_line.end());
    }

    for (int i = 0; i < child.get_height(); ++i) {
        padding_canvas.append(m_left, m_fill_char);
        padding_roles.insert(padding_roles.end(), m_left, static_cast<uint8_t>(ColorRole::Default));
        padding_canvas.append(child.get_canvas_element().data() + i * child.get_width(), child.get_width());
        padding_roles.insert(padding_roles.end(), child.get_color_roles().begin() + i * child.get_width(), child.get_color_roles().begin() + (i + 1) * child.get_width());
        padding_canvas.append(m_right, m_fill_char);
        padding_roles.insert(padding_roles.end(), m_right, static_cast<uint8_t>(ColorRole::Default));
    }

    for (int i = 0; i < m_bottom; ++i) {
        padding_canvas.append(top_bottom_padding);
        padding_roles.insert(padding_roles.end(), default_line.begin(), default_line.end());
    }

    return CanvasElement(padding_canvas, padding_roles, size);
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

bool Padding::is_dirty() const {
    return m_child->is_dirty() || m_is_dirty;
}
