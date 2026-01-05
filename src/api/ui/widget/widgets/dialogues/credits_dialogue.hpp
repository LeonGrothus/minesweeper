#pragma once
#include <memory>

#include "api/ui/widget/widget.hpp"

class CreditsDialogue : public Widget {
public:
    explicit CreditsDialogue();

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Widget> m_contructed_widget;
};