#include "alignment.hpp"

#include "api/ui/canvas/terminal_helper.hpp"

Alignment::Alignment(std::unique_ptr<Widget> child, const Position alignment)
	: m_child(std::move(child)),
	  m_alignment(alignment) {
}

const CanvasElement &Alignment::build_widget(const ElementSize &size) {
	const CanvasElement &child = m_child->build_widget(m_child->get_minimum_size());
	m_cached_canvas = position_canvas_element(child, m_alignment, size, ' ');
	return m_cached_canvas;
}

ElementSize Alignment::get_minimum_size() const {
	return m_child->get_minimum_size();
}

void Alignment::keyboard_press(const int key) {
	m_child->keyboard_press(key);
}

void Alignment::update(const double delta_time) {
	m_child->update(delta_time);
}

bool Alignment::is_dirty() const {
	return m_child->is_dirty();
}