#pragma once
#include "api/ui/widget/widget.hpp"

class Empty : public Widget {
public:
    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;
};
