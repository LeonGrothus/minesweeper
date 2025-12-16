#include "visibility.hpp"

#include "empty.hpp"

Visibility::Visibility(std::shared_ptr<Widget> child, bool visible, bool transition)
    : m_child(std::move(child)), m_visible(visible), m_transition(transition) {
    if (m_transition) {
        const auto empty = std::make_shared<Empty>();
        m_transition_widget = visible
                                  ? std::make_shared<TransitionWidget>(empty, m_child)
                                  : std::make_shared<TransitionWidget>(empty, empty);
    }
}

void Visibility::set_visible(const bool visible) {
    if (m_visible == visible) return;
    m_visible = visible;
    m_is_dirty = true;

    if (!m_transition) return;

    if (visible) {
        m_transition_widget->set_new_end(m_child);
    } else {
        m_transition_widget->set_new_end(std::make_shared<Empty>());
    }
}

bool Visibility::is_visible() const {
    return m_visible;
}

CanvasElement Visibility::build_canvas_element(const Vector2D &size) {
    if (m_transition) {
        return m_transition_widget->build_widget(size);
    }

    if (!m_visible) {
        return CanvasElement::empty(size, u' ');
    }
    return m_child->build_widget(size);
}

Vector2D Visibility::get_minimum_size() const {
    return m_child->get_minimum_size();
}

void Visibility::keyboard_press(const int key) {
    if (!m_visible) {
        return;
    }
    m_child->keyboard_press(key);
}

void Visibility::update(const double delta_time) {
    if (m_transition) {
        m_transition_widget->update(delta_time);
        return;
    }
    if (!m_visible) {
        return;
    }
    m_child->update(delta_time);
}

bool Visibility::is_dirty() const {
    if (m_transition) {
        return m_is_dirty || m_transition_widget->is_dirty();
    }
    return m_is_dirty || (m_visible && m_child->is_dirty());
}
