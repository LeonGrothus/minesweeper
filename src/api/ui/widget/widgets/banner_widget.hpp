#pragma once
#include "../widget.hpp"
#include "../../canvas/canvas_element.hpp"
#include "../../canvas/terminal_helper.hpp"

class BannerWidget final : public Widget {

public:
	explicit BannerWidget(CanvasElement banner);

	explicit BannerWidget(const std::string &banner);

	void update(double delta_time) override;

	Vector2D get_minimum_size() const override;

	void keyboard_press(int key) override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
	CanvasElement m_banner;
};