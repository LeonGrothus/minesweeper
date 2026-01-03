#include "stack.hpp"

#include <algorithm>

Stack::Stack(const std::shared_ptr<Widget> &base, const StackInfo &info) {
    add_entry(base, info);
}

void Stack::push_new_widget(const std::shared_ptr<Widget> &widget, const StackInfo &info) {
    add_entry(widget, info);
}

void Stack::pop_widget(const std::shared_ptr<Widget> &widget) {
    if (const int index = get_widget_index(widget); index > 0) {
        delete_entry(index);
    }
}

void Stack::pop_widget(const int widget_index) {
    if (widget_index > 0 && widget_index < static_cast<int>(m_stack.size())) {
        delete_entry(widget_index);
    }
}

int Stack::get_stacked_count() const {
    return static_cast<int>(m_stack.size());
}

StackInfo &Stack::get_stack_info(const std::shared_ptr<Widget> &widget) {
    return get_stack_info(get_widget_index(widget));
}

StackInfo &Stack::get_stack_info(const int widget_index) {
    return m_stack_info.at(widget_index);
}

Vector2D Stack::get_minimum_size() const {
    Vector2D size{};
    for (const std::shared_ptr<Widget> &widget: m_stack) {
        auto [widget_x, widget_y] = widget->get_minimum_size();
        size.x = std::max(size.x, widget_x);
        size.y = std::max(size.y, widget_y);
    }
    return size;
}

void Stack::keyboard_press(int key) {
    for (int i = static_cast<int>(m_stack.size()) - 1; i >= 0; i--) {
        m_stack.at(i)->keyboard_press(key);
        if (m_stack_info.at(i).take_focus) {
            return;
        }
    }
}

void Stack::update(const double delta_time) {
    for (const std::shared_ptr<Widget> &widget: m_stack) {
        widget->update(delta_time);
    }
}

bool Stack::is_dirty() const {
    return m_is_dirty || std::ranges::any_of(m_stack.begin(), m_stack.end(), [](const std::shared_ptr<Widget> &widget) {
        return widget->is_dirty();
    });
}

CanvasElement Stack::build_canvas_element(const Vector2D &size) {
    CanvasElement canvas = CanvasElement::empty(size, u' ');

    for (size_t i = 0; i < m_stack.size(); i++) {
        const StackInfo info = m_stack_info[i];
        const auto [percent_x, percent_y] = Vector2D{
            static_cast<int>(info.width_percentage * size.x), static_cast<int>(info.height_percentage * size.y)
        };
        const auto [unscaled_x, unscaled_y] = Vector2D{
            std::max(percent_x, info.absolute_size.x), std::max(percent_y, info.absolute_size.y)
        };
        const Vector2D widget_size = Vector2D{std::min(unscaled_x, size.x), std::min(unscaled_y, size.y)};

        position_element_on_canvas(m_stack[i]->build_widget(widget_size), info.alignment, canvas);
    }
    return canvas;
}

int Stack::get_widget_index(const std::shared_ptr<Widget> &widget) const {
    for (size_t i = 0; i < m_stack.size(); i++) {
        if (m_stack.at(i) == widget) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void Stack::add_entry(const std::shared_ptr<Widget> &widget, const StackInfo &info) {
    m_stack.push_back(widget);
    m_stack_info.push_back(info);
}

void Stack::delete_entry(const int index) {
    m_stack.erase(std::next(m_stack.begin(), index));
    m_stack_info.erase(std::next(m_stack_info.begin(), index));
}
