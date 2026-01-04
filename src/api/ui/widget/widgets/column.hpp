#pragma once
#include "../widget.hpp"
#include <vector>
#include <memory>

#include "row.hpp"

class Column final : public Widget {
public:
    explicit Column(std::vector<std::shared_ptr<Widget> > children);

    void set_spacing(int spacing);

    void main_axis_alignment(ListAlignment alignment);

    int get_child_count() const;

    void push_child_at(const std::shared_ptr<Widget> &child, int position);

    void push_child(const std::shared_ptr<Widget> &child);

    bool is_dirty() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    Vector2D get_minimum_size() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::vector<std::shared_ptr<Widget> > m_children;
    int m_spacing = 0;
    ListAlignment m_alignment = ListAlignment::Start;
};

