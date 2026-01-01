#pragma once
#include <optional>

#include "api/helper/looped_execution_wrapper.hpp"
#include "api/ui/widget/widget.hpp"

class Timer : public Widget {
public:
    explicit Timer(bool blink = true, bool show_millis = false);

    void reset();

    void stop();

    void resume();

    void set_blink_time(double blink_time);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    double m_current_millis = 0;
    int m_current_seconds = 0;
    int m_current_minutes = 0;

    bool m_blink;
    bool m_show_column = false;
    bool m_show_millis;

    bool m_running = true;

    double m_blink_time = 1000; //ms
};
