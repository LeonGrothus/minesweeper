#pragma once
#include <memory>

#include "api/ui/widget/widget.hpp"

class Rotation : public Widget {
public:
    explicit Rotation(const std::shared_ptr<Widget> &child, int min_rot_deg, int max_rot_deg);

    void set_loop_time(double loop_time);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;


    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Widget> m_child;

    double m_current_rot = 0;
    double m_min_rot;
    double m_max_rot;

    double m_time = 0;
    double m_loop_time = 7500; //ms
};
