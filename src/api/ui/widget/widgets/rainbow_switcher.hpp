#pragma once
#include <memory>

#include "api/helper/looped_execution_wrapper.hpp"
#include "api/ui/widget/widget.hpp"

class RainbowSwitcher : public Widget {
public:
    explicit RainbowSwitcher(const std::shared_ptr<Widget> &child, std::vector<ColorRole> roles_to_loop, bool index_random);

    void set_switch_delay(double delay);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Widget> m_child;

    bool m_index_random;
    int m_current_index = 0;
    std::vector<ColorRole> m_roles_to_loop;

    ColorRole m_new_role = ColorRole::Default;

    double m_switch_delay = 500; //ms
    LoopedExecutionWrapper m_switch_loop;
};
