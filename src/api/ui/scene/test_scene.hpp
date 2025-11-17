#pragma once

#include "scene.hpp"
#include "widgets/banner_widget.hpp"

class TestScene final : public Scene {
public:
	TestScene();

	~TestScene() override = default;

	bool is_dirty() override;

	void keyboard_press(int key) override;

	void update(double delta_time) override;

	void draw() override;

private:
	BannerWidget m_banner;
};