#pragma once
#include "keyboard_controller.hpp"
#include "api/helper/delta_timer.hpp"
#include "api/ui/scene/scene.hpp"
#include <memory>

class TerminalController {
public:
	explicit TerminalController(std::unique_ptr<Scene> default_scene);

	void run();

private:
	void draw_scene() const;

	void update_scene(double delta_time) const;

	bool m_running;
	double m_current_millis;
	Vector2D m_terminal_size;
	KeyboardController m_keyboard_controller;
	DeltaTimer m_delta_timer;
	std::unique_ptr<Scene> m_current_scene;
};
