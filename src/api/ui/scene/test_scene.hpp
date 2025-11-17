#pragma once

#include "scene.hpp"
#include "widgets/column.hpp"
#include <memory>

class TestScene final : public Scene {
public:
	TestScene();

	~TestScene() override = default;

	bool is_dirty() override;

	void keyboard_press(int key) override;

	bool update(double delta_time) override;

	void draw() override;

private:
	std::unique_ptr<Column> m_column;
	bool m_running = true;
};