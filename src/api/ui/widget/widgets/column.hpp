#pragma once
#include "../widget.hpp"
#include <vector>
#include <memory>

class Column final : public Widget {
public:
    explicit Column(std::vector<std::shared_ptr<Widget> > children);

    void set_spacing(int spacing);

    void set_alignment(TextAlignment alignment);

    bool is_dirty() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    Vector2D get_minimum_size() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::vector<std::shared_ptr<Widget> > m_children;
    int m_spacing = 0;
    TextAlignment m_alignment = TextAlignment::Left;
};

