#include "row.hpp"

Row::Row(std::vector<Widget> &children) {
	m_children = std::move(children);
}

CanvasElement Row::build_widget(ElementSize &size) const {
	const ElementSize min_size = get_minimum_size();
	CanvasElement widget("");
	if (size < min_size || m_children.empty()) {
		return widget;
	}
	int flex_width = size.width;

	int total_flex = 0;
	int flex_widgets = 0;
	for (const Widget &child : m_children) {
		const int child_flex = child.m_flex;
		flex_width -= child.get_minimum_size().width;
		if (child_flex <= 0) {
			continue;
		}
		total_flex += child_flex;
		flex_widgets++;
	}

	for (const Widget &child : m_children) {
		const int child_flex = child.m_flex;
		const int child_min_width = child.get_minimum_size().width;

		ElementSize build_size{child_min_width, size.height};
		if (child_flex > 0) {
			build_size.width += (flex_width * child_flex) / total_flex;
		}
		const CanvasElement child_element = child.build_widget(build_size);

		if (widget.get_total_length() == 0) {
			widget = child_element;
			continue;
		}
		widget.merge_right_with_other(child_element);
	}
	return widget;
}

bool Row::is_dirty() const {
	for (const Widget &child : m_children) {
		if (child.is_dirty()) {
			return true;;
		}
	}
	return false;
}

void Row::update(const double delta_time) {
	for (Widget &child : m_children) {
		child.update(delta_time);
	}
}

ElementSize Row::get_minimum_size() const {
	int max_height = 0;
	int length = 0;

	for (const Widget &child : m_children) {
		const auto [width, height] = child.get_minimum_size();
		max_height = std::max(max_height, height);
		length += width;
	}
	return ElementSize{length, max_height};
}

void Row::keyboard_press(int key) {
}