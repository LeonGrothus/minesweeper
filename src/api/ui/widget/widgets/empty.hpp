#pragma once
#include "api/ui/widget/widget.hpp"

class Empty : public Widget {
public:
    explicit Empty(Vector2D minimum_empty_size = Vector2D{1, 1}, char16_t empty_char = u' ');

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    char16_t m_empty_char;
    Vector2D m_minimum_empty_size;
};
