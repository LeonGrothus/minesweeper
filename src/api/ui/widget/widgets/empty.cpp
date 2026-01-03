#include "empty.hpp"

Empty::Empty(const Vector2D minimum_empty_size, const char16_t empty_char) : m_empty_char(empty_char),
                                                                             m_minimum_empty_size(minimum_empty_size) {
}

Vector2D Empty::get_minimum_size() const {
    return m_minimum_empty_size;
}

void Empty::keyboard_press(int key) {
}

void Empty::update(double delta_time) {
}

bool Empty::is_dirty() const {
    return m_is_dirty;
}

CanvasElement Empty::build_canvas_element(const Vector2D &size) {
    return CanvasElement::empty(size, m_empty_char);
}
