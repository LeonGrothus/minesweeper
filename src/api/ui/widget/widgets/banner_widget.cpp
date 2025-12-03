#include "banner_widget.hpp"

#include <utility>

BannerWidget::BannerWidget(CanvasElement banner) : m_banner(std::move(banner)) {
}

BannerWidget::BannerWidget(const std::string &banner) : m_banner(banner) {
}


CanvasElement BannerWidget::build_canvas_element(const Vector2D &size) {
    if (size < get_minimum_size()) {
        return CanvasElement::empty(size, u' ');
    }
    return m_banner.fill_to_size(size, u' ');
}

void BannerWidget::update(double delta_time) {
}

Vector2D BannerWidget::get_minimum_size() const {
    return m_banner.get_element_size();
}

void BannerWidget::keyboard_press(int key) {
}
