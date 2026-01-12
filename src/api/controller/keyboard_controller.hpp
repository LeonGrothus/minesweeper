#pragma once
#include <functional>
#include <vector>

class KeyboardController {
public:
    ~KeyboardController();

    void set_text_input_mode(bool active, std::function<void(int)> callback = [](const int key) {});

    bool is_text_input_active() const;

    std::vector<int> get_buffered() const;

private:
    bool m_text_input_active = false;
    std::function<void(int)> m_text_input_callback;

    int get_key() const;
};
