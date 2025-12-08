#pragma once
#include "api/ui/canvas/terminal_helper.hpp"
#include "api/ui/widget/widget.hpp"

#include <memory>

class Alignment final : public Widget {
public:
	explicit Alignment(std::shared_ptr<Widget> child, Position alignment);

	Vector2D get_minimum_size() const override;

	void keyboard_press(int key) override;

	void update(double delta_time) override;

	bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

	std::shared_ptr<Widget> m_child;
	Position m_alignment;
};