
#include "column.hpp"
#include <memory>

Column::Column(std::vector<std::shared_ptr<Widget> > children) : m_children(std::move(children)) {
}

void Column::set_spacing(const int spacing) {
    m_spacing = spacing;
}

void Column::main_axis_alignment(const ListAlignment alignment) {
    m_alignment = alignment;
    m_is_dirty = true;
}

int Column::get_child_count() const {
    return static_cast<int>(m_children.size());
}

void Column::push_child_at(const std::shared_ptr<Widget> &child, const int position) {
    if (position < 0 || position > static_cast<int>(m_children.size())) {
        return;
    }
    m_children.insert(m_children.begin() + position, child);
    m_is_dirty = true;
}

void Column::push_child(const std::shared_ptr<Widget> &child) {
    m_children.push_back(child);
    m_is_dirty = true;
}

CanvasElement Column::build_canvas_element(const Vector2D &size) {
    if (m_children.empty()) {
        return CanvasElement::empty(size, u' ');
    }

    const int spacing_total = m_spacing * (static_cast<int>(m_children.size()) - 1);

    int total_flex = 0;
    int min_height_sum = 0;
    int last_flex_index = -1;
    for (int i = 0; i < m_children.size(); i++) {
        const std::shared_ptr<Widget> &child = m_children[i];
        const int child_flex = child->m_flex;
        min_height_sum += child->get_minimum_size().y;
        if (child_flex > 0) {
            total_flex += child_flex;
            last_flex_index = i;
        }
    }

    int flex_space = size.y - min_height_sum - spacing_total;
    flex_space = std::max(flex_space, 0);

    int distributed_extra = 0;

    CanvasElement build_widget("");
    const CanvasElement spacing_canvas = CanvasElement::empty(Vector2D(size.x, m_spacing), u' ');
    bool first = true;

    for (int i = 0; i < m_children.size(); i++) {
        const std::shared_ptr<Widget> &child = m_children[i];
        const int child_flex = child->m_flex;
        const int child_min_height = child->get_minimum_size().y;

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

        Vector2D minimum_size{size.x, child_min_height};
        
        const int padding = std::max(0, flex_space_for_child);
        const int half_difference = padding / 2;
        const int top_padding = ((static_cast<uint8_t>(m_alignment) >> 2) & 0b11) * half_difference;
        const int bottom_padding = ((static_cast<uint8_t>(m_alignment) >> 0) & 0b11) * half_difference;
        int extra_padding_top = ((padding % 2 != 0 && bottom_padding == 0) ? 1 : 0);
        int extra_padding_bottom = ((padding % 2 != 0 && bottom_padding != 0) ? 1 : 0);
        
        if(top_padding == 0 && bottom_padding == 0) {
            extra_padding_top = 0;
            extra_padding_bottom = 0;
            minimum_size.y += padding;
        }

        const CanvasElement &child_element = child->build_widget(minimum_size);

        CanvasElement padded_child = child_element;
        const int total_top_padding = top_padding + extra_padding_top;
        const int total_bottom_padding = bottom_padding + extra_padding_bottom;

        if (total_top_padding > 0) {
            padded_child.merge_above_with_other(CanvasElement::empty(Vector2D(size.x, total_top_padding), u' '));
        }
        if (total_bottom_padding > 0) {
            padded_child.merge_below_with_other(CanvasElement::empty(Vector2D(size.x, total_bottom_padding), u' '));
        }

        if (first) {
            build_widget = padded_child;
            first = false;
        } else {
            build_widget.merge_below_with_other(padded_child);
        }
        if (m_spacing != 0 && i != m_children.size() - 1) {
            build_widget.merge_below_with_other(spacing_canvas);
        }
    }
    return build_widget;
}

bool Column::is_dirty() const {
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

void Column::keyboard_press(const int key) {
    for (const std::shared_ptr<Widget> &child: m_children) {
        child->keyboard_press(key);
    }
}

void Column::update(const double delta_time) {
    for (const std::shared_ptr<Widget> &child: m_children) {
        child->update(delta_time);
    }
}

Vector2D Column::get_minimum_size() const {
    int height = 0;
    int max_width = 0;

    for (const std::shared_ptr<Widget> &child: m_children) {
        const auto [width, child_height] = child->get_minimum_size();
        max_width = std::max(max_width, width);
        height += child_height;
    }
    height += m_spacing * (static_cast<int>(m_children.size()) - 1);
    return Vector2D{max_width, height};
}
