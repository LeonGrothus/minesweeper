#pragma once

#include "api/ui/widget/widget.hpp"
#include <memory>

#include "transition_widget.hpp"

class Visibility final : public Widget {
public:
    explicit Visibility(std::shared_ptr<Widget> child, bool visible = true, bool transition = true);

    void set_visible(bool visible);

    bool is_visible() const;

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Widget> m_child;
    std::shared_ptr<TransitionWidget> m_transition_widget;
    bool m_visible;
    bool m_transition;
};
