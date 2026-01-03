#include "game_scene.hpp"

#include "api/helper/conversion_helper.hpp"
#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/column.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/row.hpp"
#include "api/ui/widget/widgets/timer.hpp"
#include "api/ui/widget/widgets/border/border.hpp"

GameScene::GameScene(const std::shared_ptr<BoardWidget> &to_play) : m_board_widget(to_play) {
    m_total_flagged_builder = [](const int placed_flags, const int total_mine_count) {
        const std::u16string flag_count = utf8_to_utf16(std::to_string(placed_flags));
        const std::u16string mine_count = utf8_to_utf16(std::to_string(total_mine_count));
        return u"Discovered Mines: " + flag_count + u" / " + mine_count;
    };
    m_total_moves_builder = [](const int played_moves) {
        const std::u16string moves_count = utf8_to_utf16(std::to_string(played_moves));
        return u"Total Moves: " + moves_count;
    };

    std::vector<std::shared_ptr<Widget> > row_content;

    m_flagged_counter_widget = std::make_shared<CustomDrawer>(u"");
    m_played_moves_widget = std::make_shared<CustomDrawer>(u"");
    m_timer_widget = std::make_shared<Timer>(true, false);
    m_timer_widget->set_description_text(u"Time: ");
    update_displayed_values();

    row_content.push_back(m_flagged_counter_widget);
    row_content.push_back(m_played_moves_widget);
    row_content.push_back(m_timer_widget);

    std::vector<std::shared_ptr<Widget> > column_content;

    const std::shared_ptr<Row> display_row = std::make_shared<Row>(row_content);
    display_row->set_alignment(TextAlignment::Center);

    const std::shared_ptr<Padding> padded_row = std::make_shared<Padding>(
        std::make_shared<Border>(display_row, BorderStyle::single_line_border()), 3, 3, 1, 1);

    padded_row->m_flex = 0;
    const std::shared_ptr<Alignment> aligned_board = std::make_shared<Alignment>(to_play, MIDDLE_CENTER);

    column_content.push_back(padded_row);
    column_content.push_back(aligned_board);

    std::shared_ptr<Column> game_column = std::make_shared<Column>(column_content);
    game_column->set_alignment(TextAlignment::Center);
    m_base_widget = std::make_shared<Border>(game_column, BorderStyle::double_line_border());
}

void GameScene::handle_update(const double delta_time) {
    Scene::handle_update(delta_time);
    update_displayed_values();
    handle_win_lost();
}

void GameScene::update_displayed_values() {
    const int total_moves = m_board_widget->get_board().get_total_moves_count();
    const int total_flags = m_board_widget->get_board().get_flagged_count();
    if (m_last_moves_count != total_moves) {
        m_played_moves_widget->set_text(m_total_moves_builder(total_moves));
        m_last_moves_count = total_moves;
    }
    if (m_last_flags_count != total_flags) {
        const int total_mines = m_board_widget->get_board().get_mine_count();
        m_flagged_counter_widget->set_text(m_total_flagged_builder(total_flags, total_mines));
        m_last_flags_count = total_flags;
    }
}

void GameScene::handle_win_lost() const {
    const bool is_game_lost = m_board_widget->get_board().is_lost();
    const bool is_game_won = m_board_widget->get_board().is_won();

    if (is_game_lost || is_game_won) {
        m_timer_widget->stop();
    }
}
