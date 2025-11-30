#include "banner_widget.hpp"

#include <utility>

BannerWidget::BannerWidget(CanvasElement banner) : m_banner(std::move(banner)) {
}

BannerWidget::BannerWidget(const std::string &banner) : m_banner(banner) {
}


const CanvasElement &BannerWidget::build_widget(const Vector2D &size) {
	m_is_dirty = false;
	if (size < get_minimum_size()) {
		m_cached_canvas = CanvasElement::empty(size, ' ');
		return m_cached_canvas;
	}
	m_cached_canvas = m_banner.fill_to_size(size, ' ');
	return m_cached_canvas;
}

void BannerWidget::update(double delta_time) {
}

Vector2D BannerWidget::get_minimum_size() const {
	return m_banner.get_element_size();
}

void BannerWidget::keyboard_press(int key) {
}