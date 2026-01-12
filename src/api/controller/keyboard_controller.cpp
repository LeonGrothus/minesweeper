#include "keyboard_controller.hpp"

#include <curses.h>

#include <utility>

#include "api/ui/canvas/terminal_helper.hpp"

KeyboardController::~KeyboardController() {
    endwin();
}

void KeyboardController::set_text_input_mode(const bool active, std::function<void(int)> callback) {
    m_text_input_active = active;
    m_text_input_callback = std::move(callback);
}

bool KeyboardController::is_text_input_active() const {
    return m_text_input_active;
}

int KeyboardController::get_key() const {
    (void)this;
    return getch();
}

std::vector<int> KeyboardController::get_buffered() const {
    (void)this;
    std::vector<int> buffered;

    int key;
    while ((key = getch()) != ERR) {
        if (m_text_input_active && m_text_input_callback) {
            m_text_input_callback(key);
        } else {
            buffered.push_back(key);
        }
    }
    return buffered;
}
