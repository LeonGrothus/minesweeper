#include "enumeration_widget.hpp"

#include "api/helper/conversion_helper.hpp"
#include "api/helper/math_helper.hpp"
#include <vector>

EnumerationWidget::EnumerationWidget(const std::shared_ptr<Widget> &child, const int skip_all_sides, const int skip_to_next_label_x,
                                     const int skip_to_next_label_y) : m_child(child),
                                                                       m_skip_to_next_label_x(skip_to_next_label_x),
                                                                       m_skip_to_next_label_y(skip_to_next_label_y),
                                                                       m_skip_left(skip_all_sides),
                                                                       m_skip_right(skip_all_sides),
                                                                       m_skip_top(skip_all_sides),
                                                                       m_skip_bottom(skip_all_sides) {
}

EnumerationWidget::EnumerationWidget(const std::shared_ptr<Widget> &child, const int skip_left, const int skip_right, const int skip_top,
                                     const int skip_bottom, const int skip_to_next_label_x,
                                     const int skip_to_next_label_y) : m_child(child), m_skip_to_next_label_x(skip_to_next_label_x),
                                                                       m_skip_to_next_label_y(skip_to_next_label_y),
                                                                       m_skip_left(skip_left),
                                                                       m_skip_right(skip_right),
                                                                       m_skip_top(skip_top),
                                                                       m_skip_bottom(skip_bottom) {
}

void EnumerationWidget::set_enumeration_locations(const bool top, const bool bottom, const bool left, const bool right) {
    m_enabled_label[0] = left;
    m_enabled_label[1] = right;
    m_enabled_label[2] = top;
    m_enabled_label[3] = bottom;
}

void EnumerationWidget::set_label_spacing(const int spacing) {
    m_label_spacing = spacing;
}

Vector2D EnumerationWidget::get_minimum_size() const {
    const auto [board_x, board_y] = m_child->get_minimum_size();

    const int real_board_size_y = board_y - m_skip_top - m_skip_bottom;

    const int enumeration_size_y = real_board_size_y / (m_skip_to_next_label_y + 1);

    const int max_digits_y = digits(enumeration_size_y);

    const int x_size = board_x + (max_digits_y + m_label_spacing) * (m_enabled_label[0] + m_enabled_label[1]);
    const int y_size = board_y + (m_enabled_label[2] + m_enabled_label[3]);

    return Vector2D{x_size, y_size};
}

void EnumerationWidget::keyboard_press(const int key) {
    m_child->keyboard_press(key);
}

void EnumerationWidget::update(const double delta_time) {
    m_child->update(delta_time);
}

bool EnumerationWidget::is_dirty() const {
    return m_is_dirty || m_child->is_dirty();
}

CanvasElement EnumerationWidget::build_canvas_element(const Vector2D &size) {
    const CanvasElement &child_canvas = m_child->build_widget(m_child->get_minimum_size());

    const auto [board_x, board_y] = m_child->get_minimum_size();

    const int real_board_size_x = board_x - m_skip_left - m_skip_right;
    const int real_board_size_y = board_y - m_skip_top - m_skip_bottom;

    const int enumeration_size_x = real_board_size_x / (m_skip_to_next_label_x + 1);
    const int enumeration_size_y = real_board_size_y / (m_skip_to_next_label_y + 1);

    const int max_digits_x = digits(enumeration_size_x);
    const int max_digits_y = digits(enumeration_size_y);

    const int final_width = board_x + (max_digits_y + m_label_spacing) * (m_enabled_label[0] + m_enabled_label[1]);
    const int final_height = board_y + (m_enabled_label[2] + m_enabled_label[3]);

    std::u16string final_canvas;
    std::vector<uint8_t> final_roles;
    final_canvas.reserve(final_width * final_height);
    final_roles.reserve(final_width * final_height);

    std::u16string x_labels;
    std::vector<uint8_t> x_label_roles;
    x_labels.reserve(final_width);
    x_label_roles.reserve(final_width);

    if (m_enabled_label[0]) {
        x_labels.append(max_digits_y + m_label_spacing, u' ');
        x_label_roles.insert(x_label_roles.end(), max_digits_y + m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
    }
    x_labels.append(m_skip_left, u' ');
    x_label_roles.insert(x_label_roles.end(), m_skip_left, static_cast<uint8_t>(ColorRole::Text));
    for (int x = 0; x < enumeration_size_x; x++) {
        if (x != 0) {
            x_labels.append(m_label_spacing, u' ');
            x_label_roles.insert(x_label_roles.end(), m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
        }
        const std::u16string number = utf8_to_utf16(std::to_string(x + 1));
        x_labels.append(number);
        x_label_roles.insert(x_label_roles.end(), number.size(), static_cast<uint8_t>(ColorRole::Text));
        x_labels.append(max_digits_x - static_cast<int>(number.size()), u' ');
        x_label_roles.insert(x_label_roles.end(), max_digits_x - static_cast<int>(number.size()), static_cast<uint8_t>(ColorRole::Text));
    }
    x_labels.append(m_skip_right, u' ');
    x_label_roles.insert(x_label_roles.end(), m_skip_right, static_cast<uint8_t>(ColorRole::Text));
    if (m_enabled_label[1]) {
        x_labels.append(max_digits_y + m_label_spacing, u' ');
        x_label_roles.insert(x_label_roles.end(), max_digits_y + m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
    }
    x_labels.resize(final_width, u' ');
    x_label_roles.resize(static_cast<size_t>(final_width), static_cast<uint8_t>(ColorRole::Text));
    if (m_enabled_label[2]) {
        final_canvas.append(x_labels);
        final_roles.insert(final_roles.end(), x_label_roles.begin(), x_label_roles.end());
    }

    int current_line = 0;

    const std::u16string &bordered_lines = child_canvas.get_canvas_element();
    const std::vector<uint8_t> &bordered_roles = child_canvas.get_color_roles();
    const int bordered_width = child_canvas.get_width();

    for (int i = 0; i < m_skip_top; i++) {
        if (m_enabled_label[0]) {
            final_canvas.append(max_digits_y + m_label_spacing, u' ');
            final_roles.insert(final_roles.end(), max_digits_y + m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
        }
        final_canvas.append(std::u16string_view(bordered_lines.data() + current_line * bordered_width, bordered_width));
        final_roles.insert(final_roles.end(), bordered_roles.begin() + current_line * bordered_width, bordered_roles.begin() + (current_line + 1) * bordered_width);
        if (m_enabled_label[1]) {
            final_canvas.append(max_digits_y + m_label_spacing, u' ');
            final_roles.insert(final_roles.end(), max_digits_y + m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
        }
        current_line++;
    }

    for (int y = 0; y < enumeration_size_y; y++) {
        std::u16string_view relevant_line(bordered_lines.data() + current_line * bordered_width, bordered_width);
        const std::vector<uint8_t> relevant_roles(bordered_roles.begin() + current_line * bordered_width, bordered_roles.begin() + (current_line + 1) * bordered_width);
        const std::u16string number = utf8_to_utf16(std::to_string(y + 1));

        if (m_enabled_label[0]) {
            final_canvas.append(max_digits_y - static_cast<int>(number.size()), u' ');
            final_roles.insert(final_roles.end(), max_digits_y - static_cast<int>(number.size()), static_cast<uint8_t>(ColorRole::Text));
            final_canvas.append(number);
            final_roles.insert(final_roles.end(), number.size(), static_cast<uint8_t>(ColorRole::Text));
            final_canvas.append(m_label_spacing, u' ');
            final_roles.insert(final_roles.end(), m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
        }

        final_canvas.append(relevant_line);
        final_roles.insert(final_roles.end(), relevant_roles.begin(), relevant_roles.end());

        if (m_enabled_label[1]) {
            final_canvas.append(m_label_spacing, u' ');
            final_roles.insert(final_roles.end(), m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
            final_canvas.append(max_digits_y - static_cast<int>(number.size()), u' ');
            final_roles.insert(final_roles.end(), max_digits_y - static_cast<int>(number.size()), static_cast<uint8_t>(ColorRole::Text));
            final_canvas.append(number);
            final_roles.insert(final_roles.end(), number.size(), static_cast<uint8_t>(ColorRole::Text));
        }
        current_line++;
        for (int i = 0; i < m_skip_to_next_label_y; i++) {
            std::u16string_view relevant_sub_line(bordered_lines.data() + current_line * bordered_width, bordered_width);
            const std::vector<uint8_t> relevant_sub_roles(bordered_roles.begin() + current_line * bordered_width, bordered_roles.begin() + (current_line + 1) * bordered_width);
            if (m_enabled_label[0]) {
                final_canvas.append(max_digits_y + m_label_spacing, u' ');
                final_roles.insert(final_roles.end(), max_digits_y + m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
            }

            final_canvas.append(relevant_sub_line);
            final_roles.insert(final_roles.end(), relevant_sub_roles.begin(), relevant_sub_roles.end());

            if (m_enabled_label[1]) {
                final_canvas.append(max_digits_y + m_label_spacing, u' ');
                final_roles.insert(final_roles.end(), max_digits_y + m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
            }
            current_line++;
        }
    }

    for (int i = 0; i < m_skip_bottom; i++) {
        if (m_enabled_label[0]) {
            final_canvas.append(max_digits_y + m_label_spacing, u' ');
            final_roles.insert(final_roles.end(), max_digits_y + m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
        }
        final_canvas.append(std::u16string_view(bordered_lines.data() + current_line * bordered_width, bordered_width));
        final_roles.insert(final_roles.end(), bordered_roles.begin() + current_line * bordered_width, bordered_roles.begin() + (current_line + 1) * bordered_width);
        if (m_enabled_label[1]) {
            final_canvas.append(max_digits_y + m_label_spacing, u' ');
            final_roles.insert(final_roles.end(), max_digits_y + m_label_spacing, static_cast<uint8_t>(ColorRole::Text));
        }
        current_line++;
    }

    if (m_enabled_label[3]) {
        final_canvas.append(x_labels);
        final_roles.insert(final_roles.end(), x_label_roles.begin(), x_label_roles.end());
    }

    return CanvasElement(final_canvas, final_roles, Vector2D{final_width, final_height});
}
