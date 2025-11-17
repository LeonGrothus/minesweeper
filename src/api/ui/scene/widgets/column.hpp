#pragma once
#include "widget.hpp"
#include <vector>
#include <memory>

class Column final : public Widget {
public:
	explicit Column(std::vector<std::unique_ptr<Widget> > children);

	CanvasElement build_widget(ElementSize &size) const override;

	bool is_dirty() const override;

	void keyboard_press(int key) override;

	void update(double delta_time) override;

	ElementSize get_minimum_size() const override;

private:
	std::vector<std::unique_ptr<Widget> > m_children;
};

