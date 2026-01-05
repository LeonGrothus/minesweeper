#include "terminal_controller.hpp"

#include "api/ui/scene/scene.hpp"
#include "api/ui/canvas/terminal_helper.hpp"

#include <curses.h>
#include <memory>

#include "color_manager.hpp"
#include "api/ui/scene/transition_scene.hpp"

TerminalController::TerminalController(std::unique_ptr<Scene> default_scene)
    : m_running(true),
      m_current_millis(0),
      m_terminal_size(get_terminal_size()),
      m_current_scene(std::move(default_scene)) {
    init_terminal();

    FileManager manager(FILE_LOCATION);
    m_settings_manager = std::make_shared<SettingsManager>(manager);

    m_current_scene->set_settings_manager(m_settings_manager);
}

TerminalController::~TerminalController() {
    if (m_settings_manager) {
        m_settings_manager->save_to_file();
    }
    endwin();
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
            bool scene_changed = false;
            m_current_millis = std::max(m_current_millis - FRAME_TIME, 0.0);

            update_scene(delta_time);

            if (m_current_scene->is_exit_requested()) {
                m_running = false;
                if (m_settings_manager) {
                    m_settings_manager->save_to_file();
                }
                return;
            }

            if (std::unique_ptr<Scene> next_scene = m_current_scene->take_pending_scene()) {
                if (m_current_scene->should_use_transition()) {
                    std::shared_ptr<Widget> current_widget = m_current_scene->get_base_widget();
                    m_current_scene = std::make_unique<TransitionScene>(current_widget, std::move(next_scene));
                } else {
                    m_current_scene = std::move(next_scene);
                }
                m_current_scene->set_settings_manager(m_settings_manager);
                m_current_scene->set_dirty();
                scene_changed = true;
            }

            if (m_current_scene->is_dirty() || terminal_size_changed || scene_changed) {
                draw_scene();
            }
        }

        usleep(static_cast<int>(FRAME_TIME + 1) * 1000);
    }
}

void TerminalController::init_terminal() {
    setlocale(LC_ALL, "");
    if (!getenv("TERM")) {
        setenv("TERM", "xterm-256color", 1);
    }

    initscr();

    //remove cursor
    curs_set(0);

    init_terminal_colors();

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    set_escdelay(25);
}

void TerminalController::draw_scene() const {
    const CanvasElement &scene_canvas = m_current_scene->build_scene(m_terminal_size);

    render_to_ncurses_buffered(scene_canvas, m_terminal_size);
}

void TerminalController::update_scene(const double delta_time) const {
    for (const int key: m_keyboard_controller.get_buffered()) {
        m_current_scene->keyboard_press(key);
    }
    m_current_scene->update(delta_time);
}
