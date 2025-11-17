#pragma once
#include "keyboard_controller.hpp"
#include "api/helper/DeltaTimer.hpp"
#include "api/ui/scene/scene.hpp"
#include <memory>

class TerminalController {
public:
	explicit TerminalController(std::unique_ptr<Scene> default_scene);

	void run();

private:
	void draw_scene() const;

	void update_scene(double delta_time);

	bool m_running;
	double m_current_millis;
	KeyboardController m_keyboard_controller;
	DeltaTimer m_delta_timer;
	std::unique_ptr<Scene> m_current_scene;
};
