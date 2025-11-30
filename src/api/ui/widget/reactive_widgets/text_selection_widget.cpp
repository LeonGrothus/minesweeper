#include "text_selection_widget.hpp"
#include "../../canvas/canvas_element.hpp"
#include "api/ui/canvas/terminal_helper.hpp"

#include <algorithm>
#include <ncurses.h>

TextSelectionWidget::TextSelectionWidget(const bool loop, const bool blink_highlighted) : m_loop(loop),
	m_blink_highlighted(blink_highlighted) {
}

void TextSelectionWidget::add_option(const std::string &option, const std::function<void()> &func) {
	m_options.push_back(option);
	m_options_func.push_back(func);
	m_is_dirty = true;
}

void TextSelectionWidget::set_selected_index(const int index) {
	if (index >= 0 && index < static_cast<int>(m_options.size())) {
		m_selected_index = index;
		m_is_dirty = true;
	}
}

int TextSelectionWidget::get_selected_index() const {
	return m_selected_index;
}

std::string TextSelectionWidget::get_selected_option() const {
	if (m_selected_index >= 0 && m_selected_index < static_cast<int>(m_options.size())) {
		return m_options[m_selected_index];
	}
	return "";
}

const CanvasElement &TextSelectionWidget::build_widget(const Vector2D &size) {
	m_is_dirty = false;

	if (size < get_minimum_size() || m_options.empty()) {
		m_cached_canvas = CanvasElement::empty(size, ' ');
		return m_cached_canvas;
	}

	std::string result;
	result.reserve(size.size());

	for (size_t i = 0; i < size.y; ++i) {
		std::string line;
		line.reserve(size.x);

		if (i < m_options.size()) {
			if (i == m_selected_index) {
				line = "> ";
				if (!m_highlighted) {
					line += m_options[i];
				} else {
					line += std::string(m_options[i].size(), '#');
				}
			} else {
				line = "  ";
				line += m_options[i];
			}
		}
		if (line.size() < size.x) {
			line.append(size.x - line.size(), ' ');
		}

		line.append(size.x - line.length(), ' ');

		result += line;
	}
	m_cached_canvas = CanvasElement(result, size);
	return m_cached_canvas;
}

void TextSelectionWidget::keyboard_press(const int key) {
	switch (key) {
	case KEY_UP:
		move_selection_up();
		break;
	case KEY_DOWN:
		move_selection_down();
		break;
	case 10:
	case 13:
	case KEY_ENTER:
		m_options_func[get_selected_index()]();
		break;
	default:
		break;
	}
}

void TextSelectionWidget::update(const double delta_time) {
	if (!m_blink_highlighted) {
		return;
	}
	m_current_millis += delta_time;
	if (m_current_millis > m_millis_blink_interval) {
		m_current_millis -= m_millis_blink_interval;
		m_highlighted = !m_highlighted;
		m_is_dirty = true;
	}
}

Vector2D TextSelectionWidget::get_minimum_size() const {
	if (m_options.empty()) {
		return Vector2D{0, 0};
	}

	int max_width = 0;
	for (const std::string &option : m_options) {
		max_width = std::max(max_width, static_cast<int>(option.length()) + 4);
	}

	return Vector2D{max_width, static_cast<int>(m_options.size())};
}

void TextSelectionWidget::move_selection(const int amount) {
	if (m_options.empty()) {
		return;
	}

	m_selected_index += amount;

	const int options_size = static_cast<int>(m_options.size());
	if (m_loop) {
		m_selected_index = (m_selected_index + options_size) % options_size;
	} else {
		m_selected_index = std::clamp(m_selected_index, 0, options_size - 1);
	}

	m_current_millis = 0;
	m_highlighted = false;

	m_is_dirty = true;
}

void TextSelectionWidget::move_selection_up() {
	move_selection(-1);
}

void TextSelectionWidget::move_selection_down() {
	move_selection(1);
}