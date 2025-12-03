#pragma once
#include "../widget.hpp"

#include <memory>
#include <vector>

class Row final : public Widget {
public:
    explicit Row(std::vector<std::unique_ptr<Widget> > children);

    void set_spacing(int spacing);

    bool is_dirty() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    Vector2D get_minimum_size() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::vector<std::unique_ptr<Widget> > m_children;
    int m_spacing = 0;
};

