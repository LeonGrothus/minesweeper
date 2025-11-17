#pragma once
#include "widget.hpp"
#include <vector>

class Column : public Widget {
public:
	explicit Column(std::vector<Widget> &children);

	CanvasElement build_widget(ElementSize &size) const override;

	bool is_dirty() const override;

	void update(double delta_time) override;

	ElementSize get_minimum_size() const override;

private:
	std::vector<Widget> m_children;
};

