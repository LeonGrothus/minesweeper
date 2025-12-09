#include "visibility.hpp"

Visibility::Visibility(std::shared_ptr<Widget> child, const bool visible)
    : m_child(std::move(child)), m_visible(visible) {
}

void Visibility::set_visible(const bool visible) {
    if (m_visible == visible) {
        return;
    }
    m_visible = visible;
    m_is_dirty = true;
}

bool Visibility::is_visible() const {
    return m_visible;
}

CanvasElement Visibility::build_canvas_element(const Vector2D &size) {
    if (!m_visible) {
        return CanvasElement::empty(size, u' ');
    }
    return m_child->build_widget(size);
}

Vector2D Visibility::get_minimum_size() const {
    if (!m_visible) {
        return Vector2D{0, 0};
    }
    return m_child->get_minimum_size();
}

void Visibility::keyboard_press(const int key) {
    if (!m_visible) {
        return;
    }
    m_child->keyboard_press(key);
}

void Visibility::update(const double delta_time) {
    if (!m_visible) {
        return;
    }
    m_child->update(delta_time);
}

bool Visibility::is_dirty() const {
    return m_is_dirty || (m_visible && m_child->is_dirty());
}
