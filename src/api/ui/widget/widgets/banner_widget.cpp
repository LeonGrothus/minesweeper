#include "banner_widget.hpp"

#include <utility>

BannerWidget::BannerWidget(CanvasElement banner) : m_banner(std::move(banner)) {
}

BannerWidget::BannerWidget(const std::string &banner) : m_banner(banner) {
}

CanvasElement BannerWidget::build_canvas_element(const Vector2D &size) {
    std::vector<uint8_t> &banner_roles = m_banner.get_mutable_color_roles();
    banner_roles.assign(banner_roles.size(), m_banner_color_role);
    return m_banner;
}

void BannerWidget::set_color_role(ColorRole role) {
    m_banner_color_role = static_cast<uint8_t>(role);
    set_dirty();
}

void BannerWidget::update(double delta_time) {
}

Vector2D BannerWidget::get_minimum_size() const {
    return m_banner.get_element_size();
}

void BannerWidget::keyboard_press(int key) {
}

bool BannerWidget::is_dirty() const {
    return m_is_dirty;
}
