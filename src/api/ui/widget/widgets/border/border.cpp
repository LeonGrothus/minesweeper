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

void Border::set_color_role(ColorRole role) {
    m_border_color_role = static_cast<uint8_t>(role);
    set_dirty();
}

CanvasElement Border::build_canvas_element(const Vector2D &size) {
    if (size < get_minimum_size()) {
        return m_child->build_widget(size);
    }

    const Vector2D child_size = size - Vector2D(m_enabled_borders[2] + m_enabled_borders[3], m_enabled_borders[0] + m_enabled_borders[1]);
    const CanvasElement &child = m_child->build_widget(child_size);

    const Vector2D bordered_size = child.get_element_size() + Vector2D(m_enabled_borders[2] + m_enabled_borders[3],
                                                                       m_enabled_borders[0] + m_enabled_borders[1]);
    std::u16string border_canvas;
    border_canvas.reserve(bordered_size.area());
    std::vector<uint8_t> border_roles;
    border_roles.reserve(bordered_size.area());

    if (m_enabled_borders[0]) {
        std::u16string top_line(bordered_size.x, m_border_style.top);
        if (m_enabled_borders[2]) {
            top_line[0] = m_border_style.top_left_corner;
        }
        if (m_enabled_borders[3]) {
            top_line[bordered_size.x - 1] = m_border_style.top_right_corner;
        }
        border_canvas.append(top_line);
        border_roles.insert(border_roles.end(), bordered_size.x, m_border_color_role);
    }

    for (int i = 0; i < child.get_height(); i++) {
        if (m_enabled_borders[2]) {
            border_canvas += m_border_style.left;
            border_roles.push_back(m_border_color_role);
        }
        border_canvas.append(child.get_canvas_element().data() + i * child.get_width(), child.get_width());
        border_roles.insert(border_roles.end(), child.get_color_roles().begin() + i * child.get_width(),
                            child.get_color_roles().begin() + (i + 1) * child.get_width());
        if (m_enabled_borders[3]) {
            border_canvas += m_border_style.right;
            border_roles.push_back(m_border_color_role);
        }
    }

    if (m_enabled_borders[1]) {
        std::u16string bottom_line(bordered_size.x, m_border_style.bottom);
        if (m_enabled_borders[2]) {
            bottom_line[0] = m_border_style.bottom_left_corner;
        }
        if (m_enabled_borders[3]) {
            bottom_line[bordered_size.x - 1] = m_border_style.bottom_right_corner;
        }
        border_canvas.append(bottom_line);
        border_roles.insert(border_roles.end(), bordered_size.x, m_border_color_role);
    }

    return CanvasElement(border_canvas, border_roles, bordered_size);
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
