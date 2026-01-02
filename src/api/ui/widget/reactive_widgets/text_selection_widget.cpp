#include "text_selection_widget.hpp"
#include "../../canvas/canvas_element.hpp"

#include <algorithm>
#include <ncurses.h>

TextSelectionWidget::TextSelectionWidget(const bool loop, const bool blink_highlighted) : m_loop(loop),
                                                                                          m_blink_highlighted(blink_highlighted),
                                                                                          m_blink_loop([this]() {
                                                                                              m_highlighted = !m_highlighted;
                                                                                              m_is_dirty = true;
                                                                                          }, BLINK_INTERVAL_MS) {
}

void TextSelectionWidget::add_option(const std::u16string &option, const std::function<void()> &func) {
    m_options.push_back(option);
    m_options_func.push_back(func);
    m_is_dirty = true;
}

void TextSelectionWidget::set_selected_index(const int index) {
    if (index >= 0 && index < static_cast<int>(m_options.size())) {
        m_selected_index = index;
        m_is_dirty = true;
    }
}

int TextSelectionWidget::get_selected_index() const {
    return m_selected_index;
}

std::u16string TextSelectionWidget::get_selected_option() const {
    if (m_selected_index >= 0 && m_selected_index < static_cast<int>(m_options.size())) {
        return m_options[m_selected_index];
    }
    return u"";
}

void TextSelectionWidget::unselect() {
    m_selected = false;
}

CanvasElement TextSelectionWidget::build_canvas_element(const Vector2D &size) {
    if (size < get_minimum_size() || m_options.empty()) {
        return CanvasElement::empty(size, u' ');
    }

    std::u16string result;
    result.reserve(size.area());

    for (size_t i = 0; i < size.y; ++i) {
        std::u16string line;
        line.reserve(size.x);

        if (i < m_options.size()) {
            if (i == m_selected_index) {
                line = u"> ";
                if (!m_highlighted) {
                    line += m_options[i];
                } else {
                    line += std::u16string(m_options[i].size(), u'#');
                }
            } else {
                line = u"  ";
                line += m_options[i];
            }
        }
        if (line.size() < size.x) {
            line.append(size.x - line.size(), u' ');
        }

        line.append(size.x - line.length(), u' ');

        result += line;
    }
    return CanvasElement(result, size);
}

void TextSelectionWidget::select() {
    m_options_func[get_selected_index()]();
    m_selected = true;
    m_highlighted = true;
}

void TextSelectionWidget::keyboard_press(const int key) {
    switch (key) {
        case KEY_UP:
            move_selection_up();
            break;
        case KEY_DOWN:
            move_selection_down();
            break;
        case 10:
        case 13:
        case KEY_ENTER:
        case KEY_RIGHT:
            select();
            break;
        default:
            break;
    }
}

void TextSelectionWidget::update(const double delta_time) {
    if (!m_blink_highlighted || m_selected) {
        return;
    }

    m_blink_loop.update(delta_time);
}

Vector2D TextSelectionWidget::get_minimum_size() const {
    if (m_options.empty()) {
        return Vector2D{0, 0};
    }

    int max_width = 0;
    for (const std::u16string &option: m_options) {
        max_width = std::max(max_width, static_cast<int>(option.length()) + 4);
    }

    return Vector2D{max_width, static_cast<int>(m_options.size())};
}

bool TextSelectionWidget::is_dirty() const {
    return m_is_dirty;
}

void TextSelectionWidget::move_selection(const int amount) {
    unselect();
    if (m_options.empty()) {
        return;
    }

    m_selected_index += amount;

    const int options_size = static_cast<int>(m_options.size());
    if (m_loop) {
        m_selected_index = (m_selected_index + options_size) % options_size;
    } else {
        m_selected_index = std::clamp(m_selected_index, 0, options_size - 1);
    }

    m_highlighted = false;

    m_is_dirty = true;
}

void TextSelectionWidget::move_selection_up() {
    move_selection(-1);
}

void TextSelectionWidget::move_selection_down() {
    move_selection(1);
}
