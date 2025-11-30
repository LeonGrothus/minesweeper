#pragma once
#include "api/ui/widget/widget.hpp"

#include <memory>

class Padding final : public Widget {
public:
	explicit Padding(std::unique_ptr<Widget> child, int all_sides);

	explicit Padding(std::unique_ptr<Widget> child, int left, int right, int top, int bottom);

	void set_border_char(char border_char);

	void set_fill_char(char fill_char);

	bool is_dirty() const override;

	const CanvasElement &build_widget(const Vector2D &size) override;

	Vector2D get_minimum_size() const override;

	void keyboard_press(int key) override;

	void update(double delta_time) override;

private:
	std::unique_ptr<Widget> m_child;

	int m_left;
	int m_right;
	int m_top;
	int m_bottom;

	char m_border_char = ' ';
	char m_fill_char = ' ';
};