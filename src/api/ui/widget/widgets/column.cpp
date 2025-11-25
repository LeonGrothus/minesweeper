
#include "column.hpp"
#include <memory>

Column::Column(std::vector<std::unique_ptr<Widget> > children) : m_children(std::move(children)) {
}

const CanvasElement &Column::build_widget(const ElementSize &size) {
	const ElementSize min_size = get_minimum_size();
	m_cached_canvas = CanvasElement("");
	if (size < min_size || m_children.empty()) {
		m_cached_canvas = CanvasElement::empty(size, ' ');
		return m_cached_canvas;
	}
	int flex_height = size.height;

	int total_flex = 0;
	for (const std::unique_ptr<Widget> &child : m_children) {
		const int child_flex = child->m_flex;
		flex_height -= child->get_minimum_size().height;
		if (child_flex <= 0) {
			continue;
		}
		total_flex += child_flex;
	}

	for (const std::unique_ptr<Widget> &child : m_children) {
		const int child_flex = child->m_flex;
		const int child_min_height = child->get_minimum_size().height;

		ElementSize build_size{size.width, child_min_height};
		if (child_flex > 0 && total_flex > 0) {
			build_size.height += (flex_height * child_flex) / total_flex;
		}
		const CanvasElement &child_element = child->build_widget(build_size);

		if (m_cached_canvas.get_total_length() == 0) {
			m_cached_canvas = child_element;
			continue;
		}
		m_cached_canvas.merge_below_with_other(child_element);
	}
	return m_cached_canvas;
}

bool Column::is_dirty() const {
	for (const std::unique_ptr<Widget> &child : m_children) {
		if (child->is_dirty()) {
			return true;
		}
	}
	return false;
}

void Column::keyboard_press(const int key) {
	for (const std::unique_ptr<Widget> &child : m_children) {
		child->keyboard_press(key);
	}
}

void Column::update(const double delta_time) {
	for (const std::unique_ptr<Widget> &child : m_children) {
		child->update(delta_time);
	}
}

ElementSize Column::get_minimum_size() const {
	int height = 0;
	int max_width = 0;

	for (const std::unique_ptr<Widget> &child : m_children) {
		const auto [width, child_height] = child->get_minimum_size();
		max_width = std::max(max_width, width);
		height += child_height;
	}
	return ElementSize{max_width, height};
}