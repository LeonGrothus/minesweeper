#include "row.hpp"

Row::Row(std::vector<std::shared_ptr<Widget> > children) {
    m_children = std::move(children);
}

void Row::set_spacing(const int spacing) {
    m_spacing = spacing;
}

CanvasElement Row::build_canvas_element(const Vector2D &size) {
    if (const Vector2D min_size = get_minimum_size(); size < min_size || m_children.empty()) {
        return CanvasElement::empty(size, u' ');
    }

    int flex_width = size.x;
    int total_flex = 0;
    for (const std::shared_ptr<Widget> &child: m_children) {
        const int child_flex = child->m_flex;
        flex_width -= child->get_minimum_size().x;
        if (child_flex <= 0) {
            continue;
        }
        total_flex += child_flex;
    }

    CanvasElement build_widget("");
    const CanvasElement spacing_canvas = CanvasElement::empty(Vector2D(m_spacing, size.y), u' ');
    bool first = true;

    for (int i = 0; i < m_children.size(); i++) {
        const std::shared_ptr<Widget> &child = m_children[i];
        const int child_flex = child->m_flex;
        const int child_min_width = child->get_minimum_size().x;

        Vector2D build_size{child_min_width, size.y};
        if (child_flex > 0 && total_flex > 0) {
            build_size.x += (flex_width * child_flex) / total_flex;
        }
        const CanvasElement &child_element = child->build_widget(build_size);

        if (first) {
            build_widget = child_element;
            first = false;
        } else {
            build_widget.merge_right_with_other(child_element);
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
            return true;;
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
