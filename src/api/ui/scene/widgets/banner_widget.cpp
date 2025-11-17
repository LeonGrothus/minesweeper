#include "banner_widget.hpp"

BannerWidget::BannerWidget(const CanvasElement &banner, const Position position) : m_banner(banner),
	m_position(position) {
}

BannerWidget::BannerWidget(const std::string &banner, const Position position) : m_banner(banner),
	m_position(position) {
}


CanvasElement BannerWidget::build_widget(ElementSize &size) const {
	if (size < get_minimum_size()) {
		return CanvasElement("");
	}
	return position_canvas_element(m_banner, m_position, size, ' ');
}

bool BannerWidget::is_dirty() const {
	return true;
}

void BannerWidget::update(double delta_time) {
}

ElementSize BannerWidget::get_minimum_size() const {
	return m_banner.get_element_size();
}