#pragma once
#include "widget.hpp"
#include "api/ui/canvas_element.hpp"
#include "api/ui/terminal_helper.hpp"

class BannerWidget final : public Widget {

public:
	explicit BannerWidget(CanvasElement banner, Position position);

	explicit BannerWidget(const std::string &banner, Position position);

	CanvasElement build_widget(ElementSize &size) const override;

	bool is_dirty() const override;

	void update(double delta_time) override;

	ElementSize get_minimum_size() const override;

	void keyboard_press(int key) override;

private:
	CanvasElement m_banner;
	Position m_position;
};