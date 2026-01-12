#pragma once
#include "api/helper/looped_execution_wrapper.hpp"
#include "api/ui/widget/widget.hpp"

class TextInput : public Widget {
public:
    explicit TextInput(std::function<void(std::string)> string_callback, std::u16string suffix,
                       int max_text_length = 20);

    void on_activate();

    void on_close();

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D& size) override;

private:
    std::function<void(std::string)> m_string_callback;

    bool m_active_typing = false;

    std::u16string m_text_suffix;
    std::string m_text;
    int m_max_text_length;

    static constexpr double BLINK_INTERVAL_MS = 500.0;

    LoopedExecutionWrapper m_cursor_loop;
    bool m_cursor_visible = false;
    char16_t m_cursor_char = u'#';
};
