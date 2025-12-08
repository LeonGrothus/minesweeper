#pragma once

#include "styles/border_style.hpp"
#include <bitset>
#include <memory>

#include "api/ui/widget/widget.hpp"

class Border final : public Widget {
public:
    explicit Border(std::unique_ptr<Widget> child, const PaddingBorderStyle &style);

    void set_border_style(const PaddingBorderStyle &style);

    void set_enabled_borders(bool top, bool bottom, bool left, bool right);

    CanvasElement build_canvas_element(const Vector2D &size) override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

    Vector2D get_minimum_size() const override;

private:
    std::unique_ptr<Widget> m_child;
    PaddingBorderStyle m_border_style;
    std::bitset<4> m_enabled_borders = std::bitset<4>(0b1111); //top, bottom, left, right
};
