#include "border.hpp"

#include <memory>

#include "api/ui/widget/widget.hpp"

Border::Border(std::shared_ptr<Widget> child, const BorderStyle &style)
    : m_child(std::move(child)), m_border_style(style) {
}

void Border::set_border_style(const BorderStyle &style) {
    m_border_style = style;
}

void Border::set_enabled_borders(const bool top, const bool bottom, const bool left, const bool right) {
    m_enabled_borders[0] = top;
    m_enabled_borders[1] = bottom;
    m_enabled_borders[2] = left;
    m_enabled_borders[3] = right;
}

CanvasElement Border::build_canvas_element(const Vector2D &size) {
    if (size < get_minimum_size()) {
        return m_child->build_widget(size);
    }

    const Vector2D child_size = size - Vector2D(m_enabled_borders[2] + m_enabled_borders[3], m_enabled_borders[0] + m_enabled_borders[1]);
    const CanvasElement &child = m_child->build_widget(child_size);

    return CanvasElement::wrap_with_border(child, m_border_style, m_enabled_borders);
}

void Border::keyboard_press(const int key) {
    m_child->keyboard_press(key);
}

void Border::update(const double delta_time) {
    m_child->update(delta_time);
}

bool Border::is_dirty() const {
    return m_child->is_dirty() || m_is_dirty;
}

Vector2D Border::get_minimum_size() const {
    return m_child->get_minimum_size() + Vector2D(m_enabled_borders[2] + m_enabled_borders[3], m_enabled_borders[0] + m_enabled_borders[1]);
}
