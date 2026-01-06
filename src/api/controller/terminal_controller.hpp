#pragma once
#include "keyboard_controller.hpp"
#include "api/helper/delta_timer.hpp"
#include "api/ui/scene/scene.hpp"
#include <memory>

#include "settings_manager.hpp"


constexpr static double UPDATE_RATE = 60.0;
constexpr static double FRAME_TIME = 1000.0 / UPDATE_RATE;

constexpr static char16_t EMPTY_CHAR = u' ';
constexpr static char16_t ALPHA_CHAR = u'\x7F';

class TerminalController {
public:
    explicit TerminalController(std::unique_ptr<Scene> default_scene);

    ~TerminalController();

    void run();

private:
    static void init_terminal();

    void draw_scene() const;

    void update_scene(double delta_time) const;

    void screen_shot() const;

    std::shared_ptr<SettingsManager> m_settings_manager;

    bool m_running;
    double m_current_millis;
    Vector2D m_terminal_size;
    KeyboardController m_keyboard_controller;
    DeltaTimer m_delta_timer;
    std::unique_ptr<Scene> m_current_scene;
};
