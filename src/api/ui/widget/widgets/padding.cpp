#include "padding.hpp"

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
    padding_canvas.reserve(size.area());

    const std::u16string top_bottom_padding(size.x, m_fill_char);
    for (int i = 0; i < m_top; ++i) {
        padding_canvas.append(top_bottom_padding);
    }

    for (int i = 0; i < child.get_height(); ++i) {
        padding_canvas.append(m_left, m_fill_char);
        padding_canvas.append(child.get_canvas_element().data() + i * child.get_width(), child.get_width());
        padding_canvas.append(m_right, m_fill_char);
    }

    for (int i = 0; i < m_bottom; ++i) {
        padding_canvas.append(top_bottom_padding);
    }

    return CanvasElement(padding_canvas, size);
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
