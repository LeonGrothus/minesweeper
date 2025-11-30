#include "row.hpp"

Row::Row(std::vector<std::unique_ptr<Widget> > children) {
	m_children = std::move(children);
}

const CanvasElement &Row::build_widget(const Vector2D &size) {
	const Vector2D min_size = get_minimum_size();
	m_cached_canvas = CanvasElement("");
	if (size < min_size || m_children.empty()) {
		m_cached_canvas = CanvasElement::empty(size, ' ');
		return m_cached_canvas;
	}

	int flex_width = size.x;
	int total_flex = 0;
	for (const std::unique_ptr<Widget> &child : m_children) {
		const int child_flex = child->m_flex;
		flex_width -= child->get_minimum_size().x;
		if (child_flex <= 0) {
			continue;
		}
		total_flex += child_flex;
	}

	for (const std::unique_ptr<Widget> &child : m_children) {
		const int child_flex = child->m_flex;
		const int child_min_width = child->get_minimum_size().x;

		Vector2D build_size{child_min_width, size.y};
		if (child_flex > 0 && total_flex > 0) {
			build_size.x += (flex_width * child_flex) / total_flex;
		}
		const CanvasElement &child_element = child->build_widget(build_size);

		if (m_cached_canvas.get_total_length() == 0) {
			m_cached_canvas = child_element;
			continue;
		}
		m_cached_canvas.merge_right_with_other(child_element);
	}
	return m_cached_canvas;
}

bool Row::is_dirty() const {
	for (const std::unique_ptr<Widget> &child : m_children) {
		if (child->is_dirty()) {
			return true;;
		}
	}
	return false;
}

void Row::update(const double delta_time) {
	for (const std::unique_ptr<Widget> &child : m_children) {
		child->update(delta_time);
	}
}

Vector2D Row::get_minimum_size() const {
	int max_height = 0;
	int length = 0;

	for (const std::unique_ptr<Widget> &child : m_children) {
		const auto [width, height] = child->get_minimum_size();
		max_height = std::max(max_height, height);
		length += width;
	}
	return Vector2D{length, max_height};
}

void Row::keyboard_press(const int key) {
	for (const std::unique_ptr<Widget> &child : m_children) {
		child->keyboard_press(key);
	}
}