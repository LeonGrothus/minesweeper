#include "column.hpp"
#include <memory>

Column::Column(std::vector<std::unique_ptr<Widget> > children) : m_children(std::move(children)) {
}

CanvasElement Column::build_widget(ElementSize &size) const {
	const ElementSize min_size = get_minimum_size();
	CanvasElement widget("");
	if (size < min_size || m_children.empty()) {
		return widget;
	}
	int flex_height = size.height;

	int total_flex = 0;
	int flex_widgets = 0;
	for (const std::unique_ptr<Widget> &child : m_children) {
		const int child_flex = child->m_flex;
		flex_height -= child->get_minimum_size().height;
		if (child_flex <= 0) {
			continue;
		}
		total_flex += child_flex;
		flex_widgets++;
	}

	for (const std::unique_ptr<Widget> &child : m_children) {
		const int child_flex = child->m_flex;
		const int child_min_height = child->get_minimum_size().height;

		ElementSize build_size{size.width, child_min_height};
		if (child_flex > 0) {
			build_size.height += (flex_height * child_flex) / total_flex;
		}
		const CanvasElement child_element = child->build_widget(build_size);

		if (widget.get_total_length() == 0) {
			widget = child_element;
			continue;
		}
		widget.merge_below_with_other(child_element);
	}
	return widget;
}

bool Column::is_dirty() const {
	for (const std::unique_ptr<Widget> &child : m_children) {
		if (child->is_dirty()) {
			return true;
		}
	}
	return false;
}

void Column::keyboard_press(int key) {
	for (std::unique_ptr<Widget> &child : m_children) {
		child->keyboard_press(key);
	}
}

void Column::update(const double delta_time) {
	for (std::unique_ptr<Widget> &child : m_children) {
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