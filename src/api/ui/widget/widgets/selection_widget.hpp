#pragma once
#include "../widget.hpp"
#include "../../canvas/canvas_element.hpp"
#include "api/helper/looped_execution_wrapper.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

struct SelectionWidgetOptions {
    bool is_vertical = true;
    bool loop_selection = true;
    bool blink_highlighted = true;

    bool select_on_enter = true;
    //fake select will run the callback but don't select the widget
    bool fake_select = false;
    bool update_widget_after_selected = true;
    bool parse_keyboard_events_to_selected = true;
    bool parse_keyboard_events_to_hovered = false;
    //when the keys get parsed though if the selection widget still should handle the keyboard events
    bool react_to_keyboard_events_after_selection = false;

    int spacing_options = 0;
    int spacing_indicator_and_widget = 1;
    bool make_all_same_size = true;

    char16_t selection_char = u'>';
    char16_t highlighted_char = u'#';
};

class SelectionWidget final : public Widget {
public:
    explicit SelectionWidget(const SelectionWidgetOptions &options);

    void add_option(const std::shared_ptr<Widget> &option, const std::function<void()> &func = std::function<void()>());

    void set_selected_index(int index);

    int get_selected_index() const;

    std::shared_ptr<Widget> get_selected_option() const;

    SelectionWidgetOptions &get_selection_options();

    void select();

    void unselect();

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    Vector2D get_minimum_size() const override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    SelectionWidgetOptions m_options;

    std::vector<std::shared_ptr<Widget> > m_select_options;
    std::vector<std::function<void()> > m_select_options_func;

    int m_selected_index = 0;

    LoopedExecutionWrapper m_blink_loop;
    bool m_highlighted = false;
    bool m_selected = false;

    static constexpr double BLINK_INTERVAL_MS = 500.0; //ms

    Vector2D get_option_size_large() const;

    void move_selection(int amount);

    void move_selection_up();

    void move_selection_down();
};
