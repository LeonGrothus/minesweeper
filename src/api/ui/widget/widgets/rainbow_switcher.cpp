#include "rainbow_switcher.hpp"

#include <random>
#include "api/helper/utils.hpp"

RainbowSwitcher::RainbowSwitcher(const std::shared_ptr<Widget> &child, std::vector<ColorRole> roles_to_loop,
                                 const bool index_random) : m_child(child), m_index_random(index_random),
                                                            m_roles_to_loop(std::move(roles_to_loop)) {
    m_switch_loop.emplace([this]() {
        if (m_index_random) {
            std::mt19937 &rng = get_rng();
            std::uniform_int_distribution<int> distribution(
                0, static_cast<int>(m_roles_to_loop.size()) - 1);
            m_new_role = m_roles_to_loop.at(distribution(rng));
        } else {
            m_new_role = m_roles_to_loop.at(m_current_index);
            m_current_index = ++m_current_index % static_cast<int>(m_roles_to_loop.size());
        }
        set_dirty();
    }, m_switch_delay);
}

RainbowSwitcher::RainbowSwitcher(const std::shared_ptr<Widget> &child, const ColorRole role) : m_child(child) {
    m_new_role = role;
}

void RainbowSwitcher::set_switch_delay(const double delay) {
    m_switch_delay = delay;
    if (m_switch_loop) {
        m_switch_loop->set_repeat_time(delay);
    }
}

Vector2D RainbowSwitcher::get_minimum_size() const {
    return m_child->get_minimum_size();
}

void RainbowSwitcher::keyboard_press(const int key) {
    m_child->keyboard_press(key);
}

void RainbowSwitcher::update(const double delta_time) {
    if (m_switch_loop) {
        m_switch_loop->update(delta_time);
    }
    m_child->update(delta_time);
}

bool RainbowSwitcher::is_dirty() const {
    return m_is_dirty || m_child->is_dirty();
}

CanvasElement RainbowSwitcher::build_canvas_element(const Vector2D &size) {
    CanvasElement canvas_element = m_child->build_widget(size);
    std::vector<uint8_t> &roles = canvas_element.get_mutable_color_roles();
    roles.assign(roles.size(), static_cast<uint8_t>(m_new_role));
    return canvas_element;
}
