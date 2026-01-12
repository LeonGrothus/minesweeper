#include "timer.hpp"

#include <algorithm>

#include "api/helper/conversion_helper.hpp"
#include "api/helper/math_helper.hpp"
#include "api/ui/canvas/terminal_helper.hpp"

Timer::Timer(const bool blink, const bool show_millis) : m_blink(blink), m_show_millis(show_millis) {
}

void Timer::set_description_text(const std::u16string &text) {
    m_description_text = text;
    set_dirty();
}

void Timer::reset() {
    m_current_millis = 0;
    m_current_seconds = 0;
    m_current_minutes = 0;
    set_dirty();
}

void Timer::stop() {
    m_running = false;
    m_show_column = true;
    set_dirty();
}

void Timer::resume() {
    m_running = true;
}

void Timer::set_blink_time(const double blink_time) {
    m_blink_time = blink_time;
}

void Timer::set_show_millis(const bool show_millis) {
    m_show_millis = show_millis;
}

Vector2D Timer::get_minimum_size() const {
    int x_size = std::max(digits(m_current_minutes), 2) + 1 + 2;
    if (m_show_millis) {
        x_size += 4;
    }
    return Vector2D{x_size + static_cast<int>(m_description_text.length()), 1};
}

void Timer::update(const double delta_time) {
    if (!m_running) {
        return;
    }

    m_current_millis += delta_time;

    if (m_current_millis < 1000.0) {
        if (m_show_millis) {
            set_dirty();
        }
        return;
    }
    set_dirty();

    const int delta_seconds = static_cast<int>(m_current_millis / 1000.0);
    if (m_blink && (delta_seconds % 2 != 0)) {
        m_show_column = !m_show_column;
    }

    m_current_millis -= delta_seconds * 1000;
    m_current_seconds += delta_seconds;

    if (m_current_seconds < 60) {
        return;
    }

    const int delta_minutes = static_cast<int>(m_current_seconds / 60.0);
    m_current_seconds -= delta_minutes * 60;
    m_current_minutes += delta_minutes;
}

bool Timer::is_dirty() const {
    return m_is_dirty;
}

CanvasElement Timer::build_canvas_element(const Vector2D &size) {
    const Vector2D minimum_size = get_minimum_size();
    std::u16string full_canvas;
    full_canvas.reserve(minimum_size.area());
    full_canvas += m_description_text;

    const std::u16string minutes = utf8_to_utf16(std::to_string(m_current_minutes));
    const std::u16string seconds = utf8_to_utf16(std::to_string(m_current_seconds));

    full_canvas += std::u16string(std::max(static_cast<int>(2 - minutes.size()), 0), u'0');
    full_canvas += minutes;
    full_canvas += m_show_column ? u':' : u' ';
    full_canvas += std::u16string(std::max(static_cast<int>(2 - seconds.size()), 0), u'0');
    full_canvas += seconds;
    if (m_show_millis) {
        const std::u16string millis = utf8_to_utf16(std::to_string(static_cast<int>(m_current_millis)));
        full_canvas += '.';
        full_canvas += std::u16string(std::max(static_cast<int>(3 - millis.size()), 0), u'0');
        full_canvas += millis;
    }
    return CanvasElement(full_canvas, minimum_size, ColorRole::Text);
}
