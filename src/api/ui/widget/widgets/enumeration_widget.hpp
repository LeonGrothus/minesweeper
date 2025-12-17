#pragma once
#include <memory>

#include "api/ui/widget/widget.hpp"

class EnumerationWidget : public Widget {
public:
    explicit EnumerationWidget(const std::shared_ptr<Widget> &child, int skip_all_sides, int skip_to_next_label_x,
                               int skip_to_next_label_y);

    explicit EnumerationWidget(const std::shared_ptr<Widget> &child, int skip_left, int skip_right, int skip_top, int skip_bottom,
                               int skip_to_next_label_x,
                               int skip_to_next_label_y);

    void set_enumeration_locations(bool top, bool bottom, bool left, bool right);

    void set_label_spacing(int spacing);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Widget> m_child;

    int m_label_spacing = 1;

    int m_skip_to_next_label_x;
    int m_skip_to_next_label_y;

    int m_skip_left;
    int m_skip_right;
    int m_skip_top;
    int m_skip_bottom;
    std::bitset<4> m_enabled_label = std::bitset<4>(0b0101); //left, right, top, bottom
};
