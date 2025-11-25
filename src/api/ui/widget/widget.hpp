#pragma once
#include "../canvas/canvas_element.hpp"

enum Flex {
	NO_FLEX = 0,
	DEFAULT_FLEX = 1
};

class Widget {
public:
	virtual ~Widget() = 0;

	virtual const CanvasElement &build_widget(const ElementSize &size) = 0;

	virtual ElementSize get_minimum_size() const = 0;

	virtual void keyboard_press(int key) = 0;

	virtual void update(double delta_time) = 0;

	virtual bool is_dirty() const {
		return m_is_dirty;
	}

	int m_flex = DEFAULT_FLEX;

protected:
	bool m_is_dirty = true;
	CanvasElement m_cached_canvas{CanvasElement("")};
};

inline Widget::~Widget() = default;