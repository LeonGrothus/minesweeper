#pragma once

#include "border_style.hpp"
#include <bitset>
#include <memory>

#include "api/ui/widget/widget.hpp"

class Border final : public Widget {
public:
    explicit Border(std::shared_ptr<Widget> child, const BorderStyle &style);

    void set_border_style(const BorderStyle &style);

    void set_enabled_borders(bool top, bool bottom, bool left, bool right);

    void set_color_role(ColorRole role);

    CanvasElement build_canvas_element(const Vector2D &size) override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

    Vector2D get_minimum_size() const override;

private:
    std::shared_ptr<Widget> m_child;
    BorderStyle m_border_style;
    std::bitset<4> m_enabled_borders = std::bitset<4>(0b1111); //top, bottom, left, right

    uint8_t m_border_color_role = static_cast<uint8_t>(ColorRole::Text);
};
