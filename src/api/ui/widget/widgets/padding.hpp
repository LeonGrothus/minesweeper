#pragma once
#include "api/ui/widget/widget.hpp"

#include <memory>

class Padding final : public Widget {
public:
    explicit Padding(std::unique_ptr<Widget> child, int all_sides);

    explicit Padding(std::unique_ptr<Widget> child, int left, int right, int top, int bottom);

    void set_fill_char(char16_t fill_char);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::unique_ptr<Widget> m_child;

    int m_left;
    int m_right;
    int m_top;
    int m_bottom;

    char16_t m_fill_char = u' ';
};
