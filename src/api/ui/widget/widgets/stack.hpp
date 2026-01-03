#pragma once
#include <memory>

#include "../widget.hpp"
#include "alignment.hpp"

struct StackInfo {
    //if both percentage and absolute size both are set then the largest that still fits will be taken
    double width_percentage = 1;
    double height_percentage = 1;
    Vector2D absolute_size = {-1, -1};

    Position alignment = MIDDLE_CENTER;

    bool take_focus = false;
};

class Stack : public Widget {
public:
    explicit Stack(const std::shared_ptr<Widget> &base, const StackInfo &info);

    void push_new_widget(const std::shared_ptr<Widget> &widget, const StackInfo &info);

    void pop_widget(const std::shared_ptr<Widget> &widget);

    void pop_widget(int widget_index);

    int get_stacked_count() const;

    StackInfo &get_stack_info(const std::shared_ptr<Widget> &widget);

    StackInfo &get_stack_info(int widget_index);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::vector<std::shared_ptr<Widget> > m_stack = std::vector<std::shared_ptr<Widget> >();
    std::vector<StackInfo> m_stack_info = std::vector<StackInfo>();

    int get_widget_index(const std::shared_ptr<Widget> &widget) const;

    void add_entry(const std::shared_ptr<Widget> &widget, const StackInfo &info);

    void delete_entry(int index);
};
