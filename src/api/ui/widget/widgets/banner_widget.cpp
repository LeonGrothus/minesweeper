#include "banner_widget.hpp"

#include <utility>

#include "api/helper/file_manager.hpp"

BannerWidget::BannerWidget(CanvasElement banner) : m_banner(std::move(banner)) {
}

BannerWidget::BannerWidget(const std::string &file_path) : m_banner(u"") {
    const FileManager manager(file_path);
    std::string content;
    if (!manager.file_exists() || !manager.read_string_content(content)) {
        show_temporary_message("FAILED TO LOAD ASSET", 1000);
        return;
    }

    m_banner = CanvasElement(content);
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

Vector2D BannerWidget::get_minimum_size() const {
    return m_banner.get_element_size();
}

bool BannerWidget::is_dirty() const {
    return m_is_dirty;
}
