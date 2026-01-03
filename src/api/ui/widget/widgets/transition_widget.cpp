#include "transition_widget.hpp"

#include <algorithm>
#include <memory>
#include <random>
#include <utility>

#include "empty.hpp"
#include "api/ui/canvas/canvas_element.hpp"
#include "api/ui/canvas/terminal_helper.hpp"

TransitionWidget::TransitionWidget(const std::shared_ptr<Widget> &end, const bool fade_in)
    : m_transition_loop([this]() {
          handle_next_transitions(1);
          set_dirty();
      }, 1.0),
      m_start_widget(fade_in ? std::make_shared<Empty>() : end),
      m_end_widget(end),
      m_start_canvas(""),
      m_end_canvas("") {
    if (!fade_in) {
        m_transition_finished = true;
    }
}

TransitionWidget::TransitionWidget(const std::shared_ptr<Widget> &start, const std::shared_ptr<Widget> &end) : m_transition_loop([this]() {
        handle_next_transitions(1);
        set_dirty();
    }, 1.0),
    m_start_widget(start), m_end_widget(end), m_start_canvas(""),
    m_end_canvas("") {
}

void TransitionWidget::set_new_end(const std::shared_ptr<Widget> &new_end) {
    if (m_end_widget == new_end) {
        return;
    }

    if (!m_transition_finished) {
        m_break_between_transition = true;
    }

    m_is_transition_initialized = false;
    m_end_widget = new_end;
    m_cover_indices.clear();
    m_uncover_indices.clear();
    m_to_change = 0;
    m_char_reveal_time = 0;
    m_transition_finished = false;
    m_transition_loop.reset();

    m_start_canvas = m_cached_canvas;
    m_set_new_end = true;
    set_dirty();
}

bool TransitionWidget::is_transition_finished() const {
    return m_transition_finished;
}

void TransitionWidget::set_transition_time(const double transition_time) {
    m_transition_time = transition_time;
}

void TransitionWidget::set_transition_char_color_role(ColorRole role) {
    m_transition_char_color_role = static_cast<uint8_t>(role);
    set_dirty();
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
    
    m_transition_loop.update(delta_time);
}


bool TransitionWidget::is_dirty() const {
    return m_is_dirty || (m_transition_finished && m_end_widget->is_dirty());
}

CanvasElement TransitionWidget::build_canvas_element(const Vector2D &size) {
    if (m_transition_finished) {
        return m_end_widget->build_widget(size);
    }

    //to change is 0 when transition needs to start
    if (m_to_change == 0) {
        if (!m_set_new_end) {
            m_start_canvas = m_start_widget->build_widget(size);
        } else {
            if (m_start_canvas.get_element_size() != size) {
                m_start_canvas = m_start_canvas.fill_to_size(size, u' ');
            }
        }
        m_set_new_end = false;
        m_end_canvas = m_end_widget->build_widget(size);

        m_cover_indices.clear();
        m_uncover_indices.clear();
        m_to_change = 0;
        m_transition_finished = false;
        m_transition_loop.reset();

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
    m_is_transition_initialized = true;
    m_canvas_size = size;
    const std::u16string &start_canvas = m_start_canvas.get_canvas_element();
    const std::u16string &end_canvas = m_end_canvas.get_canvas_element();

    std::vector<int> transition_chars{};
    std::vector<int> non_transition_chars{};

    for (int i = 0; i < size.area(); i++) {
        if (start_canvas.at(i) == end_canvas.at(i)) {
            continue;
        }
        if (start_canvas.at(i) == m_transition_char) {
            transition_chars.push_back(i);
        } else {
            non_transition_chars.push_back(i);
        }
    }

    m_to_change = static_cast<int>(transition_chars.size() + non_transition_chars.size());

    //skip transition when no transition needed to avoid div by 0
    if (m_to_change == 0) {
        m_transition_finished = true;
        m_cached_canvas = m_end_canvas;
        m_char_reveal_time = 0;
        return;
    }

    //if no chars to cover go straight to uncovering
    if (non_transition_chars.empty()) {
        m_cover_indices.clear();
        std::ranges::shuffle(transition_chars, std::random_device());
        m_uncover_indices = std::move(transition_chars);
    } else {
        //covered chars need to be uncovered
        std::vector<int> all_to_uncover(transition_chars);
        all_to_uncover.insert(all_to_uncover.end(), non_transition_chars.begin(), non_transition_chars.end());

        std::ranges::shuffle(non_transition_chars, std::random_device());
        m_cover_indices = std::move(non_transition_chars);

        std::ranges::shuffle(all_to_uncover, std::random_device());
        m_uncover_indices = std::move(all_to_uncover);
    }

    m_char_reveal_time = m_transition_time / (2 * m_to_change);
    m_cached_canvas = m_start_canvas;

    m_transition_loop.set_repeat_time(m_char_reveal_time);
}

void TransitionWidget::handle_next_transitions(const int count) {
    if (!m_is_transition_initialized) {
        return;
    }
    for (int i = 0; i < count; i++) {
        if (!m_cover_indices.empty()) {
            const int index = m_cover_indices.back();
            m_cover_indices.pop_back();

            m_cached_canvas.get_mutable_canvas_element().at(index) = m_transition_char;
            m_cached_canvas.get_mutable_color_roles().at(index) = m_transition_char_color_role;
        } else if (!m_uncover_indices.empty()) {
            const int index = m_uncover_indices.back();
            m_uncover_indices.pop_back();

            m_cached_canvas.get_mutable_canvas_element().at(index) = m_end_canvas.get_canvas_element().at(index);
            m_cached_canvas.get_mutable_color_roles().at(index) = m_end_canvas.get_color_roles().at(index);
        } else {
            m_transition_finished = true;
            break;
        }
    }
}
