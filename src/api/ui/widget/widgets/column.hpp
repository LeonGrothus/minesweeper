#pragma once
#include "../widget.hpp"
#include <vector>
#include <memory>

class Column final : public Widget {
public:
    explicit Column(std::vector<std::unique_ptr<Widget> > children);

    void set_spacing(int spacing);

    const CanvasElement &build_widget(const Vector2D &size) override;

    bool is_dirty() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    Vector2D get_minimum_size() const override;

private:
    std::vector<std::unique_ptr<Widget> > m_children;
    int m_spacing = 0;
};

