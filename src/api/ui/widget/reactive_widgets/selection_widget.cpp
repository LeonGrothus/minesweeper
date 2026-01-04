#include "selection_widget.hpp"
#include "../../canvas/canvas_element.hpp"

#include <algorithm>
#include <ncurses.h>

#include "api/controller/terminal_controller.hpp"
#include "api/ui/widget/widgets/empty.hpp"


SelectionWidget::SelectionWidget(
    const SelectionWidgetOptions &options) : m_options(options), m_blink_loop([this]() {
    m_highlighted = !m_highlighted;
    m_is_dirty = true;
}, BLINK_INTERVAL_MS) {
}

void SelectionWidget::add_option(const std::shared_ptr<Widget> &option, const std::function<void()> &func) {
    m_select_options.push_back(option);
    m_select_options_func.push_back(func);
    m_is_dirty = true;
}

void SelectionWidget::set_selected_index(const int index) {
    if (index >= 0 && index < static_cast<int>(m_select_options.size())) {
        m_selected_index = index;
        m_is_dirty = true;
    }
}

int SelectionWidget::get_selected_index() const {
    return m_selected_index;
}

std::shared_ptr<Widget> SelectionWidget::get_selected_option() const {
    if (m_selected_index >= 0 && m_selected_index < static_cast<int>(m_select_options.size())) {
        return m_select_options[m_selected_index];
    }
    return nullptr;
}

void SelectionWidget::select() {
    m_select_options_func[get_selected_index()]();
    m_selected = true;
    m_highlighted = true;
}

void SelectionWidget::unselect() {
    m_selected = false;
}

CanvasElement SelectionWidget::build_canvas_element(const Vector2D &size) {
    if (m_select_options.empty()) {
        return CanvasElement::empty(size, EMPTY_CHAR);
    }

    const Vector2D widget_size = get_option_size_large();
    auto [actual_size_x, actual_size_y] = widget_size;
    actual_size_x += 1 + m_options.spacing_indicator_and_widget;

    bool first = true;
    CanvasElement final_canvas("");
    CanvasElement empty_canvas("");
    CanvasElement selection_space = CanvasElement::empty(Vector2D{m_options.spacing_indicator_and_widget, widget_size.y}, EMPTY_CHAR);
    CanvasElement not_selected = CanvasElement::empty(Vector2D{1, widget_size.y}, EMPTY_CHAR);
    CanvasElement selected = CanvasElement::empty(Vector2D{1, widget_size.y}, m_options.selection_char);

    if (m_select_options.size() > 1) {
        if (m_options.is_vertical) {
            empty_canvas = CanvasElement::empty(Vector2D{actual_size_x, m_options.spacing_options}, EMPTY_CHAR);
        } else {
            empty_canvas = CanvasElement::empty(Vector2D{m_options.spacing_options, actual_size_y}, EMPTY_CHAR);
        }
    }

    int options_size = static_cast<int>(m_select_options.size());
    for (int i = 0; i < options_size; i++) {
        const std::shared_ptr<Widget> &option = m_select_options[i];

        CanvasElement widget_canvas("");

        Vector2D minimum_option_size = option->get_minimum_size();
        if (m_highlighted && i == get_selected_index()) {
            if (m_options.make_all_same_size) {
                widget_canvas = CanvasElement::empty(widget_size, m_options.highlighted_char);
            } else {
                widget_canvas = CanvasElement::empty(minimum_option_size, m_options.highlighted_char).fill_to_size(widget_size, EMPTY_CHAR);
            }
        } else {
            if (m_options.make_all_same_size) {
                widget_canvas = option->build_widget(widget_size);
            } else {
                widget_canvas = option->build_widget(minimum_option_size).fill_to_size(widget_size, EMPTY_CHAR);
            }
        }
        if (m_options.spacing_indicator_and_widget > 0) {
            widget_canvas.merge_left_with_other(selection_space);
        }

        widget_canvas.merge_left_with_other(get_selected_index() == i ? selected : not_selected);

        if (first) {
            final_canvas = std::move(widget_canvas);
            first = false;
        } else {
            if (m_options.is_vertical) {
                final_canvas.merge_below_with_other(widget_canvas);
            } else {
                final_canvas.merge_right_with_other(widget_canvas);
            }
        }

        if (i != options_size - 1) {
            if (m_options.is_vertical) {
                final_canvas.merge_above_with_other(empty_canvas);
            } else {
                final_canvas.merge_left_with_other(empty_canvas);
            }
        }
    }
    return final_canvas;
}


void SelectionWidget::keyboard_press(const int key) {
    if (m_selected) {
        if (m_options.parse_keyboard_events_to_selected) {
            m_select_options[get_selected_index()]->keyboard_press(key);
        }
        if (!m_options.react_to_keyboard_events_after_selection) {
            return;
        }
    }
    switch (key) {
        case KEY_UP:
            if (m_options.is_vertical) {
                move_selection_up();
            }
            break;
        case KEY_RIGHT:
            if (!m_options.is_vertical) {
                move_selection_down();
            }
            break;
        case KEY_DOWN:
            if (m_options.is_vertical) {
                move_selection_down();
            }
            break;
        case KEY_LEFT:
            if (!m_options.is_vertical) {
                move_selection_up();
            }
            break;
        case 10:
        case 13:
        case KEY_ENTER:
            if (m_options.select_on_enter) {
                select();
                set_dirty();
            }
            break;
        //esc key
        case 27:
            m_selected = false;
            break;
        default:
            break;
    }
}

void SelectionWidget::update(const double delta_time) {
    if (m_selected) {
        if (m_options.update_widget_after_selected) {
            return m_select_options[get_selected_index()]->update(delta_time);
        }
    }

    if (!m_options.blink_highlighted || m_selected) {
        return;
    }
    m_blink_loop.update(delta_time);
}

Vector2D SelectionWidget::get_minimum_size() const {
    if (m_select_options.empty()) {
        return Vector2D{0, 0};
    }
    Vector2D minimum_size = get_option_size_large();
    //for selection indicator
    minimum_size.x += 1 + m_options.spacing_indicator_and_widget;

    const int option_length = static_cast<int>(m_select_options.size());
    const int spacing = m_options.spacing_options * (option_length - 1);

    if (m_options.is_vertical) {
        minimum_size.y = minimum_size.y * option_length + spacing;
    } else {
        minimum_size.x = minimum_size.x * option_length + spacing;
    }

    return minimum_size;
}

bool SelectionWidget::is_dirty() const {
    return m_is_dirty;
}

Vector2D SelectionWidget::get_option_size_large() const {
    Vector2D minimum_size{0, 0};

    for (const std::shared_ptr<Widget> &option: m_select_options) {
        auto [min_x, min_y] = option->get_minimum_size();
        minimum_size.x = std::max(minimum_size.x, min_x);
        minimum_size.y = std::max(minimum_size.y, min_y);
    }

    return minimum_size;
}

void SelectionWidget::move_selection(const int amount) {
    unselect();
    if (m_select_options.empty()) {
        return;
    }

    m_selected_index += amount;

    const int options_size = static_cast<int>(m_select_options.size());
    if (m_options.loop_selection) {
        m_selected_index = (m_selected_index + options_size) % options_size;
    } else {
        m_selected_index = std::clamp(m_selected_index, 0, options_size - 1);
    }

    m_highlighted = false;

    m_is_dirty = true;
}

void SelectionWidget::move_selection_up() {
    move_selection(-1);
}

void SelectionWidget::move_selection_down() {
    move_selection(1);
}
