#include "list_setting.hpp"

#include <algorithm>
#include <utility>
#include <ncurses.h>

#include "api/controller/terminal_controller.hpp"

ListSetting::ListSetting(std::u16string description, const bool loop_selection) : m_description(std::move(description)),
                                                                                  m_loop_selection(loop_selection) {
}

void ListSetting::add_option(const ListSettingOption &option) {
    m_options.push_back(option);
}

void ListSetting::add_option_at_index(const ListSettingOption &option, const int index) {
    m_options.insert(m_options.begin() + index, option);
}

void ListSetting::remove_option(const ListSettingOption &option) {
    const int index = get_index_of_option(option);
    if (index == -1) {
        return;
    }
    m_options.erase(m_options.begin() + index);
}

void ListSetting::remove_option(const int index) {
    if (index < 0 || index >= static_cast<int>(m_options.size())) {
        return;
    }
    m_options.erase(m_options.begin() + index);
}

int ListSetting::get_options_count() const {
    return static_cast<int>(m_options.size());
}

int ListSetting::get_index_of_option(const ListSettingOption &option) const {
    for (int i = 0; i < static_cast<int>(m_options.size()); i++) {
        if (m_options[i] == option) {
            return i;
        }
    }
    return -1;
}

void ListSetting::set_current_index(const int index) {
    m_current_option = std::clamp(index, 0, static_cast<int>(m_options.size()) - 1);
}

const ListSettingOption &ListSetting::get_current_option() {
    return m_options[m_current_option];
}

void ListSetting::set_description_spacing(const int spacing) {
    m_description_spacing = spacing;
}

void ListSetting::set_min_options_spacing(const int spacing) {
    m_min_options_spacing = spacing;
}

Vector2D ListSetting::get_minimum_size() const {
    const int description_spacing = std::max(m_description_spacing, static_cast<int>(m_description.size()));
    const int option_spacing = get_options_size();

    //+2 due to the < > that will be added if more than one option is present
    return Vector2D{description_spacing + 2 + option_spacing, 1};
}

void ListSetting::keyboard_press(const int key) {
    switch (key) {
        case KEY_RIGHT:
            move_selection_down();
            break;
        case KEY_LEFT:
            move_selection_up();
            break;
        case 10:
        case 13:
        case KEY_ENTER:
            move_selection_down();
            break;
        default:
            break;
    }
}

void ListSetting::update(const double delta_time) {
}

bool ListSetting::is_dirty() const {
    return m_is_dirty;
}

CanvasElement ListSetting::build_canvas_element(const Vector2D &size) {
    const int options_count = static_cast<int>(m_options.size());
    if (options_count == 0) {
        return CanvasElement::empty(size, EMPTY_CHAR);
    }
    const Vector2D minimum_size = get_minimum_size();
    const int option_spacing = get_options_size();

    std::u16string final_canvas;
    final_canvas.reserve(minimum_size.area());

    const int description_length = static_cast<int>(m_description.size());
    final_canvas.append(m_description);
    const int description_spacing = std::max(m_description_spacing, description_length);
    final_canvas.append(description_spacing - description_length, EMPTY_CHAR);

    final_canvas += (options_count == 1) ? u' ' : u'<';
    const std::u16string &current_option = m_options[m_current_option].option;
    const int diff_option_size = option_spacing - static_cast<int>(current_option.size());

    const std::u16string left_padding(diff_option_size / 2, EMPTY_CHAR);
    const std::u16string right_padding((diff_option_size + 1) / 2, EMPTY_CHAR);
    final_canvas.append(left_padding);
    final_canvas.append(current_option);
    final_canvas.append(right_padding);

    final_canvas += (options_count == 1) ? u' ' : u'>';
    return CanvasElement(final_canvas, minimum_size);
}

int ListSetting::get_options_size() const {
    int option_spacing = m_min_options_spacing;

    for (const ListSettingOption &option: m_options) {
        option_spacing = std::max(option_spacing, static_cast<int>(option.option.size()));
    }
    return option_spacing;
}

void ListSetting::move_selection(const int amount) {
    if (m_options.empty()) {
        return;
    }

    m_options[m_current_option].deselect();
    m_current_option += amount;

    const int options_size = static_cast<int>(m_options.size());
    if (m_loop_selection) {
        m_current_option = (m_current_option + options_size) % options_size;
    } else {
        m_current_option = std::clamp(m_current_option, 0, options_size - 1);
    }
    m_options[m_current_option].select();
    m_is_dirty = true;
}

void ListSetting::move_selection_up() {
    move_selection(-1);
}

void ListSetting::move_selection_down() {
    move_selection(1);
}
