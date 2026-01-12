#include "custom_drawer.hpp"

#include <utility>


CustomDrawer::CustomDrawer(const std::u16string &text, const char16_t delimiter, const TextAlignment alignment) {
    set_text(text, delimiter, alignment);
}

void CustomDrawer::set_text(const std::u16string &new_text, const char16_t delimiter, const TextAlignment alignment) {
    m_custom_canvas_element = CanvasElement(new_text, delimiter, ColorRole::Text, alignment);
    set_dirty();
}

CustomDrawer::CustomDrawer(CanvasElement custom_element) : m_custom_canvas_element(std::move(custom_element)) {
}

void CustomDrawer::set_canvas_element(CanvasElement new_element) {
    m_custom_canvas_element = std::move(new_element);
}

Vector2D CustomDrawer::get_minimum_size() const {
    return m_custom_canvas_element.get_element_size();
}

bool CustomDrawer::is_dirty() const {
    return m_is_dirty;
}

CanvasElement CustomDrawer::build_canvas_element(const Vector2D &size) {
    return m_custom_canvas_element;
}
