#pragma once
#include <optional>

#include "api/ui/widget/widget.hpp"

class CustomDrawer : public Widget {
public:
    explicit CustomDrawer(const std::u16string &text, char16_t delimiter = u'\x7F', TextAlignment alignment = TextAlignment::Center);

    explicit CustomDrawer(CanvasElement custom_element);

    void set_text(const std::u16string &new_text, char16_t delimiter = u'\x7F', TextAlignment alignment = TextAlignment::Center);

    void set_canvas_element(CanvasElement new_element);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    CanvasElement m_custom_canvas_element = CanvasElement::empty(Vector2D{0, 0}, u' ');
};
