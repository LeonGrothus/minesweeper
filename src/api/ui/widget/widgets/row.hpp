#pragma once
#include "../widget.hpp"

#include <memory>
#include <vector>

enum class ListAlignment : uint8_t {
    None = 0b00000000, // child gets the full space and does not get padded automatically
    Start = 0b00000010, // 0 left, 2 right
    Center = 0b00000101, // 1 left, 1 right
    End = 0b00001000 // 2 left, 0 right
};

class Row final : public Widget {
public:
    explicit Row(std::vector<std::shared_ptr<Widget> > children);

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
    ListAlignment m_alignment = ListAlignment::None;
};

