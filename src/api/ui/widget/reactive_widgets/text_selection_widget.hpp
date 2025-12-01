#pragma once
#include "../widget.hpp"
#include "../../canvas/canvas_element.hpp"
#include "api/helper/delta_timer.hpp"

#include <functional>
#include <string>
#include <vector>

class TextSelectionWidget final : public Widget {
public:
    explicit TextSelectionWidget(bool loop = false, bool blink_highlighted = false);

    void add_option(const std::string &option, const std::function<void()> &func);

    void set_selected_index(int index);

    int get_selected_index() const;

    std::string get_selected_option() const;

    const CanvasElement &build_widget(const Vector2D &size) override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    Vector2D get_minimum_size() const override;

private:
    std::vector<std::string> m_options;

    std::vector<std::function<void()> > m_options_func;

    int m_selected_index = 0;
    bool m_loop;

    bool m_blink_highlighted;
    double m_current_millis = 0;
    bool m_highlighted = false;
    static constexpr double BLINK_INTERVAL_MS = 500.0; //ms

    void move_selection(int amount);

    void move_selection_up();

    void move_selection_down();
};
