#include "empty.hpp"

Vector2D Empty::get_minimum_size() const {
    return Vector2D{0, 0};
}

void Empty::keyboard_press(int key) {
}

void Empty::update(double delta_time) {
}

bool Empty::is_dirty() const {
    return m_is_dirty;
}

CanvasElement Empty::build_canvas_element(const Vector2D &size) {
    return CanvasElement::empty(size, ' ');
}
