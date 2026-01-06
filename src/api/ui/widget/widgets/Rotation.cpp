#include "Rotation.hpp"

#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "api/controller/terminal_controller.hpp"
#include "api/helper/math_helper.hpp"

Rotation::Rotation(const std::shared_ptr<Widget> &child, const int min_rot_deg, const int max_rot_deg) : m_child(child),
    m_min_rot(deg_to_rad(min_rot_deg)), m_max_rot(deg_to_rad(max_rot_deg)) {
}

void Rotation::set_loop_time(const double loop_time) {
    if (loop_time < 0) {
        return;
    }
    m_loop_time = loop_time;
}

Vector2D Rotation::get_minimum_size() const {
    auto [rot_x, rot_y] = m_child->get_minimum_size();
    const double cos_val = fabs(cos(m_current_rot));
    const double sin_val = fabs(sin(m_current_rot));

    //ceil both
    const int new_x = static_cast<int>(rot_x * cos_val + rot_y * sin_val + 1);
    const int new_y = static_cast<int>(rot_x * sin_val + rot_y * cos_val + 1);

    return Vector2D{new_x, new_y};
}

void Rotation::keyboard_press(const int key) {
    m_child->keyboard_press(key);
}

void Rotation::update(const double delta_time) {
    m_child->update(delta_time);

    const double mean_value = (m_min_rot + m_max_rot) / 2;
    const double size = mean_value - m_max_rot;

    m_current_rot = size * sin((m_time / m_loop_time) * 2 * M_PI) + mean_value;

    m_time += delta_time;
    if (m_time > m_loop_time) {
        m_time -= m_loop_time;
    }
    set_dirty();
}

bool Rotation::is_dirty() const {
    return m_is_dirty || m_child->is_dirty();
}

CanvasElement Rotation::build_canvas_element(const Vector2D &size) {
    const Vector2D minimum_size = get_minimum_size();
    const Vector2D half_minimum_size = minimum_size / 2;
    const Vector2D child_size = m_child->get_minimum_size();
    const Vector2D half_child_size = child_size / 2;

    const CanvasElement &child_canvas = m_child->build_widget(child_size);
    const std::u16string &child_string = child_canvas.get_canvas_element();
    const std::vector<uint8_t> &child_colors = child_canvas.get_color_roles();

    std::u16string final_canvas(minimum_size.area(), EMPTY_CHAR);
    std::vector<uint8_t> final_color(minimum_size.area(), static_cast<uint8_t>(ColorRole::Default));

    for (int i = 0; i < child_size.area(); i++) {
        const char16_t child_char = child_string.at(i);
        const uint8_t child_color = child_colors.at(i);

        Vector2D char_vec{i % child_size.x, i / child_size.x};
        auto [rot_x, rot_y] = rotate_around(char_vec - half_child_size, m_current_rot) + half_minimum_size;

        const size_t index = static_cast<size_t>(std::clamp(rot_y * minimum_size.x + rot_x, 0, minimum_size.area() - 1));
        final_canvas.at(index) = child_char;
        final_color.at(index) = child_color;
    }
    return CanvasElement(std::move(final_canvas), std::move(final_color), minimum_size);
}
