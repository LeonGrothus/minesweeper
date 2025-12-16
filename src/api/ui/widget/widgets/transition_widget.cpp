#include "transition_widget.hpp"

#include <algorithm>
#include <memory>
#include <random>
#include <utility>

TransitionWidget::TransitionWidget(const std::shared_ptr<Widget> &end) : m_start_widget(end), m_end_widget(end), m_start_canvas(""),
                                                                         m_end_canvas("") {
    m_transition_finished = true;
}

TransitionWidget::TransitionWidget(const std::shared_ptr<Widget> &start, const std::shared_ptr<Widget> &end) : m_start_widget(start),
    m_end_widget(end), m_start_canvas(""), m_end_canvas("") {
}

void TransitionWidget::set_new_end(const std::shared_ptr<Widget> &new_end) {
    m_end_widget = new_end;
    m_cover_indices.clear();
    m_uncover_indices.clear();
    m_to_change = 0;
    m_passed_time = 0;
    m_char_reveal_time = 0;
    m_transition_finished = false;

    m_start_canvas = m_cached_canvas;
    m_break_between = true;
    set_dirty();
}

void TransitionWidget::set_transition_time(const float transition_time) {
    m_transition_time = transition_time;
}

void TransitionWidget::set_transition_char(const char16_t transition_char) {
    m_transition_char = transition_char;
}

Vector2D TransitionWidget::get_minimum_size() const {
    auto [start_x, start_y] = m_start_widget->get_minimum_size();
    auto [end_x, end_y] = m_end_widget->get_minimum_size();
    return Vector2D{std::max(start_x, end_x), std::max(start_y, end_y)};
}

void TransitionWidget::keyboard_press(const int key) {
    if (!m_transition_finished) {
        return;
    }
    m_end_widget->keyboard_press(key);
}

void TransitionWidget::update(const double delta_time) {
    if (m_transition_finished) {
        m_end_widget->update(delta_time);
        return;
    }

    if (m_to_change == 0 || m_char_reveal_time == 0) {
        return;
    }

    m_passed_time += delta_time;

    handle_next_transitions(static_cast<int>(std::floor(m_passed_time / m_char_reveal_time)));
    m_passed_time = std::fmod(m_passed_time, m_char_reveal_time);

    set_dirty();
}


bool TransitionWidget::is_dirty() const {
    return m_is_dirty;
}

CanvasElement TransitionWidget::build_canvas_element(const Vector2D &size) {
    if (!m_transition_finished && m_to_change == 0) {
        if (!m_break_between) {
            m_start_canvas = m_start_widget->build_widget(size);
        } else {
            if (m_start_canvas.get_element_size() != size) {
                m_start_canvas = m_start_canvas.fill_to_size(size, u' ');
            }
        }
        m_break_between = false;
        m_end_canvas = m_end_widget->build_widget(size);

        m_cover_indices.clear();
        m_uncover_indices.clear();
        m_to_change = 0;
        m_passed_time = 0;
        m_transition_finished = false;

        init_transition(size);
    }

    if (m_canvas_size != size) {
        m_start_canvas = m_start_widget->build_widget(size);

        m_end_canvas = m_end_widget->build_widget(size);
        const double percentage = (2.0 * m_to_change + static_cast<double>(-m_cover_indices.size() - m_uncover_indices.size())) /
                                  m_to_change;
        init_transition(size);
        handle_next_transitions(static_cast<int>(2 * m_to_change * percentage));
    }

    return m_cached_canvas;
}

void TransitionWidget::init_transition(const Vector2D size) {
    m_canvas_size = size;
    const std::u16string &start_canvas = m_start_canvas.get_canvas_element();
    const std::u16string &end_canvas = m_end_canvas.get_canvas_element();

    m_to_change = 0;

    std::vector<int> to_change{};
    for (int i = 0; i < size.area(); i++) {
        if (start_canvas.at(i) == end_canvas.at(i)) {
            continue;
        }
        m_to_change++;
        to_change.push_back(i);
    }

    if (m_to_change == 0) {
        m_transition_finished = true;
        m_cached_canvas = m_end_canvas;
        m_char_reveal_time = 0;
        return;
    }

    std::ranges::shuffle(to_change, std::random_device());
    m_cover_indices = std::vector(to_change);
    std::ranges::shuffle(to_change, std::random_device());
    m_uncover_indices = std::move(to_change);

    m_char_reveal_time = m_transition_time / m_to_change;
    m_cached_canvas = m_start_canvas;
}

void TransitionWidget::handle_next_transitions(const int count) {
    for (int i = 0; i < count; i++) {
        m_passed_time -= m_char_reveal_time;
        if (!m_cover_indices.empty()) {
            const int index = m_cover_indices.back();
            m_cover_indices.pop_back();

            m_cached_canvas.get_mutable_canvas_element().at(index) = m_transition_char;
        } else if (!m_uncover_indices.empty()) {
            const int index = m_uncover_indices.back();
            m_uncover_indices.pop_back();

            m_cached_canvas.get_mutable_canvas_element().at(index) = m_end_canvas.get_canvas_element().at(index);
        } else {
            m_transition_finished = true;
            break;
        }
    }
}
