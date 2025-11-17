#pragma once
#include "api/ui/canvas_element.hpp"

constexpr int NO_FLEX = 0;
constexpr int DEFAULT = 1;

class Widget {
public:
	virtual ~Widget() = 0;

	virtual CanvasElement build_widget(ElementSize &size) const = 0;

	virtual bool is_dirty() const = 0;

	virtual void update(double delta_time) = 0;

	virtual ElementSize get_minimum_size() const = 0;

	int m_flex = DEFAULT;
};