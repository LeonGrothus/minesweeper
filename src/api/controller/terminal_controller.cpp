#include "terminal_controller.hpp"

#include "api/ui/canvas/terminal_helper.hpp"

#include <ncurses.h>

constexpr double FRAME_RATE = 60.0;
constexpr double FRAME_TIME = 1000.0 / FRAME_RATE;

TerminalController::TerminalController(std::unique_ptr<Scene> default_scene)
	: m_running(true),
	  m_current_millis(0),
	  m_terminal_size(get_terminal_size()),
	  m_current_scene(std::move(default_scene)) {
}

void TerminalController::run() {
	while (m_running) {
		const Vector2D new_terminal_size = get_terminal_size();
		const bool terminal_size_changed = new_terminal_size != m_terminal_size;
		m_terminal_size = new_terminal_size;

		m_delta_timer.update();
		const double delta_time = m_delta_timer.get_delta_millis();
		m_current_millis += delta_time;

		if (m_current_millis >= FRAME_TIME || terminal_size_changed) {
			m_current_millis = std::max(m_current_millis - FRAME_TIME, 0.0);

			update_scene(delta_time);
			if (m_current_scene->is_dirty() || terminal_size_changed) {
				draw_scene();
			}
		}

		usleep(static_cast<int>(FRAME_TIME + 1) * 1000);
	}
}

void TerminalController::draw_scene() const {
	const CanvasElement &scene_canvas = m_current_scene->build_scene(m_terminal_size);

	render_to_ncurses(scene_canvas.get_canvas_element(), m_terminal_size);
}

void TerminalController::update_scene(const double delta_time) const {
	for (const int key : m_keyboard_controller.get_buffered()) {
		m_current_scene->keyboard_press(key);
	}
	m_current_scene->update(delta_time);
}