
#include "column.hpp"
#include <memory>

Column::Column(std::vector<std::shared_ptr<Widget> > children) : m_children(std::move(children)) {
}

void Column::set_spacing(const int spacing) {
    m_spacing = spacing;
}

CanvasElement Column::build_canvas_element(const Vector2D &size) {
    if (const Vector2D min_size = get_minimum_size(); size < min_size || m_children.empty()) {
        return CanvasElement::empty(size, u' ');
    }
    int flex_height = size.y;

    int total_flex = 0;
    for (const std::shared_ptr<Widget> &child: m_children) {
        const int child_flex = child->m_flex;
        flex_height -= child->get_minimum_size().y;
        if (child_flex <= 0) {
            continue;
        }
        total_flex += child_flex;
    }
    flex_height -= m_spacing * (static_cast<int>(m_children.size()) - 1);


    CanvasElement build_widget("");
    const CanvasElement spacing_canvas = CanvasElement::empty(Vector2D(size.x, m_spacing), u' ');
    bool first = true;

    for (int i = 0; i < m_children.size(); i++) {
        const std::shared_ptr<Widget> &child = m_children[i];
        const int child_flex = child->m_flex;
        const int child_min_height = child->get_minimum_size().y;

        Vector2D build_size{size.x, child_min_height};
        if (child_flex > 0 && total_flex > 0) {
            build_size.y += (flex_height * child_flex) / total_flex;
        }
        const CanvasElement &child_element = child->build_widget(build_size);

        if (first) {
            build_widget = child_element;
            first = false;
        } else {
            build_widget.merge_below_with_other(child_element);
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
