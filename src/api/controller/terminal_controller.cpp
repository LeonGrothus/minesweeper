#include "terminal_controller.hpp"

#include <iostream>
#include <bits/this_thread_sleep.h>

constexpr double FRAME_RATE = 1000;

TerminalController::TerminalController(std::unique_ptr<Scene> default_scene)
	: m_running(true),
	  m_current_millis(0),
	  m_current_scene(std::move(default_scene)) {
}

void TerminalController::run() {
	while (m_running) {
		m_delta_timer.update();
		const double delta_time = m_delta_timer.get_delta_millis();
		m_current_millis += delta_time;

		if (m_current_millis >= FRAME_RATE) {
			m_current_millis -= FRAME_RATE;
			update_scene(delta_time);
			if (m_current_scene->is_dirty()) {
				draw_scene();
			}
		}

		usleep(1 * 1000);
	}
}

void TerminalController::draw_scene() const {
	m_current_scene->draw();
}

void TerminalController::update_scene(const double delta_time) const {
	for (const int key : m_keyboard_controller.get_buffered()) {
		m_current_scene->keyboard_press(key);
	}

	m_current_scene->update(delta_time);
}