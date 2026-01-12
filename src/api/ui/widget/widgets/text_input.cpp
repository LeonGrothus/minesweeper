#include "text_input.hpp"

#include <curses.h>

#include <utility>

#include "api/controller/keyboard_controller.hpp"
#include "api/controller/terminal_controller.hpp"
#include "api/helper/conversion_helper.hpp"

TextInput::TextInput(std::function<void(std::string)> string_callback, std::u16string suffix,
                     const int max_text_length) : m_string_callback(std::move(string_callback)),
                                                  m_text_suffix(std::move(suffix)),
                                                  m_max_text_length(max_text_length),
                                                  m_cursor_loop([this]() {
                                                      m_cursor_visible = !m_cursor_visible;
                                                      set_dirty();
                                                  }, BLINK_INTERVAL_MS) {}

void TextInput::on_activate() {
    if (m_keyboard_controller) {
        m_keyboard_controller->set_text_input_mode(true, [this](const int key) {
            if (key == 27 || key == 10 || key == KEY_ENTER) {
                //esc key or enter key
                m_string_callback(m_text);
                on_close();
            } else if (key >= 32 && key <= 126) {
                if (static_cast<int>(m_text.length()) >= m_max_text_length) {
                    return;
                }
                //alphanumeric characters
                m_text += static_cast<char>(key);
                set_dirty();
            } else if (key == 127 || key == 8 || key == 263) {
                if (!m_text.empty()) {
                    m_text.pop_back();
                    set_dirty();
                }
            }
        });
        m_active_typing = true;
        set_dirty();
    }
}

void TextInput::on_close() {
    if (m_keyboard_controller) {
        m_keyboard_controller->set_text_input_mode(false);
        m_active_typing = false;
        set_dirty();
    }
}

Vector2D TextInput::get_minimum_size() const {
    return Vector2D{static_cast<int>(m_text_suffix.length()) + m_max_text_length, 1};
}

void TextInput::keyboard_press(const int key) {
    Widget::keyboard_press(key);
}

void TextInput::update(const double delta_time) {
    m_cursor_loop.update(delta_time);
    Widget::update(delta_time);
}

bool TextInput::is_dirty() const {
    return m_is_dirty;
}

CanvasElement TextInput::build_canvas_element(const Vector2D& size) {
    const Vector2D minimum_size = get_minimum_size();

    std::u16string final_canvas;
    final_canvas.reserve(minimum_size.area());

    final_canvas.append(m_text_suffix);
    final_canvas.append(utf8_to_utf16(m_text));

    if (const int length = static_cast<int>(final_canvas.length()); length != minimum_size.x) {
        if (m_cursor_visible && m_active_typing) {
            final_canvas += m_cursor_char;
        }

        final_canvas.append(minimum_size.x - (length + static_cast<int>(m_cursor_visible && m_active_typing)),
                            EMPTY_CHAR);
    }

    return CanvasElement(final_canvas, minimum_size);
}
