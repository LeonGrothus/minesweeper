#include "text_selection_widget.hpp"
#include "api/ui/canvas_element.hpp"

#include <algorithm>
#include <ncurses.h>

TextSelectionWidget::TextSelectionWidget(const bool loop) : m_loop(loop) {
}

void TextSelectionWidget::add_option(const std::string &option, const std::function<void()> func) {
	m_options.push_back(option);
	m_options_func.push_back(func);
	m_dirty = true;
}

void TextSelectionWidget::set_selected_index(const int index) {
	if (index >= 0 && index < static_cast<int>(m_options.size())) {
		m_selected_index = index;
		m_dirty = true;
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

CanvasElement TextSelectionWidget::build_widget(ElementSize &size) const {
	const ElementSize minimum_size = get_minimum_size();
	if (size < minimum_size || m_options.empty()) {
		return CanvasElement("");
	}

	m_dirty = false;

	std::string result;
	result.reserve(minimum_size.length());
	for (int i = 0; i < size.height; ++i) {
		std::string line;
		line.reserve(size.width);

		if (i == m_selected_index) {
			line = "> ";
		} else {
			line = "  ";
		}

		line += m_options[i];
		line.append(size.width - line.length(), ' ');
		result += line;
	}

	return CanvasElement(result, size);
}

bool TextSelectionWidget::is_dirty() const {
	return m_dirty;
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

void TextSelectionWidget::update(double delta_time) {
}

ElementSize TextSelectionWidget::get_minimum_size() const {
	if (m_options.empty()) {
		return ElementSize{0, 0};
	}

	int max_width = 0;
	for (const std::string &option : m_options) {
		max_width = std::max(max_width, static_cast<int>(option.length()) + 4);
	}

	return ElementSize{max_width, static_cast<int>(m_options.size())};
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

	m_dirty = true;
}

void TextSelectionWidget::move_selection_up() {
	move_selection(-1);
}

void TextSelectionWidget::move_selection_down() {
	move_selection(1);
}