#pragma once
#include "api/ui/widget/widget.hpp"

class Timer : public Widget {
public:
    explicit Timer(bool blink = true, bool show_millis = false);

    void set_description_text(const std::u16string& text);

    int get_time_in_millis() const;

    void set_time_from_millis(int millis);

    void reset();

    void stop();

    void resume();

    void set_blink_time(double blink_time);

    void set_show_millis(bool show_millis);

    Vector2D get_minimum_size() const override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D& size) override;

private:
    double m_current_millis = 0;
    int m_current_seconds = 0;
    int m_current_minutes = 0;

    bool m_blink;
    bool m_show_column = false;
    bool m_show_millis = false;

    bool m_running = true;

    double m_blink_time = 1000; //ms

    std::u16string m_description_text;
};
