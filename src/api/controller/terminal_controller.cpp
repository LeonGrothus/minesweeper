#include "terminal_controller.hpp"

#include "api/ui/scene/scene.hpp"
#include "api/ui/canvas/terminal_helper.hpp"

#include <curses.h>
#include <memory>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

#include "color_manager.hpp"
#include "api/helper/conversion_helper.hpp"
#include "api/ui/scene/transition_scene.hpp"

TerminalController::TerminalController(const std::shared_ptr<KeyboardController>& controller,
                                       std::unique_ptr<Scene> default_scene)
    : m_running(true),
      m_current_millis(0),
      m_terminal_size(get_terminal_size()),
      m_keyboard_controller(controller),
      m_current_scene(std::move(default_scene)) {
    init_terminal();

    FileManager manager(FILE_LOCATION);
    m_settings_manager = std::make_shared<SettingsManager>(manager);

    FileManager score_board_file("score_board.bin");
    m_score_board_manager = std::make_shared<ScoreBoardManager>(score_board_file);

    m_current_scene->set_settings_manager(m_settings_manager);
    m_current_scene->set_keyboard_controller(m_keyboard_controller);
    m_current_scene->set_score_board_manager(m_score_board_manager);
}

TerminalController::~TerminalController() {
    if (m_settings_manager) {
        m_settings_manager->save_to_file();
    }
    endwin();
}


void TerminalController::run() {
    while (m_running) {
#ifdef _WIN32
        const bool terminal_size_changed = false;
#elifdef __linux__
        const Vector2D new_terminal_size = get_terminal_size();
        const bool terminal_size_changed = new_terminal_size != m_terminal_size;
        m_terminal_size = new_terminal_size;
#endif

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
                m_current_scene->set_keyboard_controller(m_keyboard_controller);
                m_current_scene->set_score_board_manager(m_score_board_manager);
                m_current_scene->set_dirty();
                scene_changed = true;
            }

            if (m_current_scene->is_dirty() || terminal_size_changed || scene_changed) {
                draw_scene();
            }
        }
#ifdef __linux__
        usleep(static_cast<int>(FRAME_TIME + 1) * 1000);
#elifdef _WIN32
        //scuffed but not easily changeable
        const Vector2D new_terminal_size = get_terminal_size();
        m_terminal_size = new_terminal_size;
        Sleep(static_cast<int>(FRAME_TIME + 1));
#endif
    }
}

void TerminalController::init_terminal() {
    setlocale(LC_ALL, "");

#ifdef __linux__
    if (!getenv("TERM")) {
        setenv("TERM", "xterm-256color", 1);
    }
#endif

    initscr();

    //remove cursor
    curs_set(0);

    init_terminal_colors();

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

#ifdef __linux__
    set_escdelay(25);
#endif
}

void TerminalController::draw_scene() const {
    const CanvasElement& scene_canvas = m_current_scene->build_scene(m_terminal_size);

#ifdef __linux__
    render_to_ncurses_buffered(scene_canvas, m_terminal_size);
#elifdef _WIN32
    render_to_ncurses_debug_only(scene_canvas, m_terminal_size);
#endif
}

void TerminalController::update_scene(const double delta_time) const {
    m_current_scene->update(delta_time);

    for (const int key : m_keyboard_controller->get_buffered()) {
        if (m_keyboard_controller->is_text_input_active()) {
            continue;
        }
        switch (key) {
        case 'P':
        case 'p':
            screen_shot();
            break;
        default:
            break;
        }

        m_current_scene->keyboard_press(key);
    }
}

void TerminalController::screen_shot() const {
    const std::string screenshot_prefix = "screen_shot_";
    const std::string screenshot_suffix = ".txt";
    uint8_t screenshot_index = 0;
    FileManager manager("");

    do {
        manager = FileManager(screenshot_prefix + std::to_string(screenshot_index) + screenshot_suffix);
    } while (manager.file_exists() && screenshot_index++ < 255);

    const std::u16string& canvas_string = m_current_scene->build_scene(m_terminal_size).
                                                           to_default_printable_string(u'\n');
    manager.write_string_content(utf16_to_utf8(canvas_string));
}
