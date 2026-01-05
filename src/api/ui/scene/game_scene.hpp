#pragma once
#include "scene.hpp"
#include "../widget/widgets/boards/board_widget.hpp"
#include "api/ui/widget/widgets/custom_drawer.hpp"
#include "api/ui/widget/widgets/timer.hpp"

class Column;

class GameScene : public Scene {
public:
    void handle_update(double delta_time) override;

    explicit GameScene(const std::shared_ptr<BoardWidget> &to_play);

protected:
    void handle_key(int key) override;

    void settings_manager_set() override;

private:
    void update_displayed_values();

    void handle_win_lost();

    void open_pause_menu();

    bool m_game_finished = false;

    std::shared_ptr<BoardWidget> m_board_widget;
    std::shared_ptr<Timer> m_timer_widget;
    std::shared_ptr<Column> m_game_column;

    std::shared_ptr<CustomDrawer> m_flagged_counter_widget;
    std::shared_ptr<CustomDrawer> m_played_moves_widget;

    int m_last_moves_count = -1;
    int m_last_flags_count = -1;

    std::function<std::u16string(int, int)> m_total_flagged_builder;
    std::function<std::u16string(int)> m_total_moves_builder;
};
