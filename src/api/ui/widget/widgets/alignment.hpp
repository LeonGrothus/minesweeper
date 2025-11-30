#pragma once
#include "api/ui/canvas/terminal_helper.hpp"
#include "api/ui/widget/widget.hpp"

#include <memory>

class Alignment final : public Widget {
public:
	explicit Alignment(std::unique_ptr<Widget> child, Position alignment);

	const CanvasElement &build_widget(const Vector2D &size) override;

	Vector2D get_minimum_size() const override;

	void keyboard_press(int key) override;

	void update(double delta_time) override;

	bool is_dirty() const override;

protected:
	std::unique_ptr<Widget> m_child;
	Position m_alignment;
};