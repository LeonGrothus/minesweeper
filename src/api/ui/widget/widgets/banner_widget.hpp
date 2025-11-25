#pragma once
#include "../widget.hpp"
#include "../../canvas/canvas_element.hpp"
#include "../../canvas/terminal_helper.hpp"

class BannerWidget final : public Widget {

public:
	explicit BannerWidget(CanvasElement banner);

	explicit BannerWidget(const std::string &banner);

	const CanvasElement &build_widget(const ElementSize &size) override;

	void update(double delta_time) override;

	ElementSize get_minimum_size() const override;

	void keyboard_press(int key) override;

private:
	CanvasElement m_banner;
};