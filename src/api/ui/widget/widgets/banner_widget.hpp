#pragma once
#include "../widget.hpp"
#include "../../canvas/canvas_element.hpp"

class BannerWidget final : public Widget {
public:
    explicit BannerWidget(CanvasElement banner);

    explicit BannerWidget(const std::string &file_path);

    void set_color_role(ColorRole role);

    void update(double delta_time) override;

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    CanvasElement m_banner;

    uint8_t m_banner_color_role = static_cast<uint8_t>(ColorRole::Text);
};
