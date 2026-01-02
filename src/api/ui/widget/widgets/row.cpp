#include "row.hpp"
#include <algorithm>

Row::Row(std::vector<std::shared_ptr<Widget> > children) {
    m_children = std::move(children);
}

void Row::set_spacing(const int spacing) {
    m_spacing = spacing;
}

void Row::set_alignment(const TextAlignment alignment) {
    m_alignment = alignment;
    m_is_dirty = true;
}

CanvasElement Row::build_canvas_element(const Vector2D &size) {
    if (m_children.empty()) {
        return CanvasElement::empty(size, u' ');
    }

    const int spacing_total = m_spacing * (static_cast<int>(m_children.size()) - 1);

    int total_flex = 0;
    int min_width_sum = 0;
    int last_flex_index = -1;
    for (int i = 0; i < m_children.size(); i++) {
        const std::shared_ptr<Widget> &child = m_children[i];
        const int child_flex = child->m_flex;
        min_width_sum += child->get_minimum_size().x;
        if (child_flex > 0) {
            total_flex += child_flex;
            last_flex_index = i;
        }
    }

    int flex_space = size.x - min_width_sum - spacing_total;
    flex_space = std::max(flex_space, 0);

    int distributed_extra = 0;

    CanvasElement build_widget("");
    const CanvasElement spacing_canvas = CanvasElement::empty(Vector2D(m_spacing, size.y), u' ');
    bool first = true;

    for (int i = 0; i < m_children.size(); i++) {
        const std::shared_ptr<Widget> &child = m_children[i];
        const int child_flex = child->m_flex;
        const int child_min_width = child->get_minimum_size().x;

        int flex_space_for_child = 0;
        if (child_flex > 0) {
            flex_space_for_child = (flex_space * child_flex) / total_flex;
            if (i == last_flex_index) {
                //remaining space due to integer division
                const int remaining_space = flex_space - (distributed_extra + flex_space_for_child);
                flex_space_for_child += remaining_space;
            }
            distributed_extra += flex_space_for_child;
        }

        const Vector2D minimum_size{child_min_width, size.y};
        const CanvasElement &child_element = child->build_widget(minimum_size);

        const int padding = std::max(0, flex_space_for_child);
        const int half_difference = padding / 2;
        const int left_padding = ((static_cast<uint8_t>(m_alignment) >> 2) & 0b11) * half_difference;
        const int right_padding = ((static_cast<uint8_t>(m_alignment) >> 0) & 0b11) * half_difference;
        const int extra_padding_left = ((padding % 2 != 0 && right_padding == 0) ? 1 : 0);
        const int extra_padding_right = ((padding % 2 != 0 && right_padding != 0) ? 1 : 0);

        CanvasElement padded_child = child_element;
        const int total_left_padding = left_padding + extra_padding_left;
        const int total_right_padding = right_padding + extra_padding_right;

        if (total_left_padding > 0) {
            padded_child.merge_left_with_other(CanvasElement::empty(Vector2D(total_left_padding, size.y), u' '));
        }
        if (total_right_padding > 0) {
            padded_child.merge_right_with_other(CanvasElement::empty(Vector2D(total_right_padding, size.y), u' '));
        }

        if (first) {
            build_widget = padded_child;
            first = false;
        } else {
            build_widget.merge_right_with_other(padded_child);
        }
        if (m_spacing != 0 && i != m_children.size() - 1) {
            build_widget.merge_right_with_other(spacing_canvas);
        }
    }
    return build_widget;
}

bool Row::is_dirty() const {
    for (const std::shared_ptr<Widget> &child: m_children) {
        if (child->is_dirty()) {
            return true;
        }
    }
    if (m_is_dirty) {
        return true;
    }
    return false;
}

void Row::update(const double delta_time) {
    for (const std::shared_ptr<Widget> &child: m_children) {
        child->update(delta_time);
    }
}

Vector2D Row::get_minimum_size() const {
    int max_height = 0;
    int length = 0;

    for (const std::shared_ptr<Widget> &child: m_children) {
        const auto [width, height] = child->get_minimum_size();
        max_height = std::max(max_height, height);
        length += width;
    }
    length += m_spacing * (static_cast<int>(m_children.size()) - 1);
    return Vector2D{length, max_height};
}

void Row::keyboard_press(const int key) {
    for (const std::shared_ptr<Widget> &child: m_children) {
        child->keyboard_press(key);
    }
}
