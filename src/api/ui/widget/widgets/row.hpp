#pragma once
#include "../widget.hpp"

#include <memory>
#include <vector>

class Row final : public Widget {
public:
	explicit Row(std::vector<std::unique_ptr<Widget> > children);

	const CanvasElement &build_widget(const ElementSize &size) override;

	bool is_dirty() const override;

	void keyboard_press(int key) override;

	void update(double delta_time) override;

	ElementSize get_minimum_size() const override;

private:
	std::vector<std::unique_ptr<Widget> > m_children;
};

