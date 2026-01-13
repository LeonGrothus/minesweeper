#include "game_scene.hpp"

#include <curses.h>

#include "main_selection_scene.hpp"
#include "api/controller/color_manager.hpp"
#include "api/controller/score_board.hpp"
#include "api/helper/conversion_helper.hpp"
#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/column.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/rainbow_switcher.hpp"
#include "api/ui/widget/widgets/row.hpp"
#include "api/ui/widget/widgets/timer.hpp"
#include "api/ui/widget/widgets/border/border.hpp"
#include "api/ui/widget/widgets/dialogues/controls_dialogue.hpp"
#include "api/ui/widget/widgets/dialogues/inform_dialogue.hpp"
#include "api/ui/widget/widgets/dialogues/input_dialogue.hpp"
#include "api/ui/widget/widgets/dialogues/simple_dialogue.hpp"

GameScene::GameScene(const std::shared_ptr<BoardWidget>& to_play, const int size_key, const int difficulty_key)
    : m_size_key(size_key), m_difficulty_key(difficulty_key), m_board_widget(to_play) {
    m_total_flagged_builder = [](const int placed_flags, const int total_mine_count) {
        const std::u16string flag_count = utf8_to_utf16(std::to_string(placed_flags));
        const std::u16string mine_count = utf8_to_utf16(std::to_string(total_mine_count));
        return u"Discovered Mines: " + flag_count + u" / " + mine_count;
    };
    m_total_moves_builder = [](const int played_moves) {
        const std::u16string moves_count = utf8_to_utf16(std::to_string(played_moves));
        return u"Total Moves: " + moves_count;
    };

    std::vector<std::shared_ptr<Widget>> row_content;

    m_flagged_counter_widget = std::make_shared<CustomDrawer>(u"");
    m_played_moves_widget = std::make_shared<CustomDrawer>(u"");
    m_timer_widget = std::make_shared<Timer>(true, false);
    m_timer_widget->set_description_text(u"Time: ");
    update_displayed_values();

    row_content.push_back(m_flagged_counter_widget);
    row_content.push_back(m_played_moves_widget);
    row_content.push_back(m_timer_widget);

    std::vector<std::shared_ptr<Widget>> column_content;

    const std::shared_ptr<Row> display_row = std::make_shared<Row>(row_content);
    display_row->main_axis_alignment(ListAlignment::Center);

    const std::shared_ptr<Padding> padded_row = std::make_shared<Padding>(
        std::make_shared<Border>(display_row, BorderStyle::single_line_border()), 3, 3, 1, 1);

    padded_row->m_flex = 0;
    const std::shared_ptr<Alignment> aligned_board = std::make_shared<Alignment>(to_play, MIDDLE_CENTER);

    column_content.push_back(padded_row);
    column_content.push_back(aligned_board);

    m_game_column = std::make_shared<Column>(column_content);
    m_game_column->main_axis_alignment(ListAlignment::Center);
    m_base_widget = std::make_shared<Border>(m_game_column, BorderStyle::double_line_border());
}

void GameScene::handle_key(const int key) {
    Scene::handle_key(key);
    switch (key) {
    case 10:
    case 13:
    case KEY_ENTER:
        if (m_game_finished) {
            request_scene_change_with_transition(std::make_unique<MainSelectionScene>());
        }
        break;
    //esc key
    case 27:
        open_pause_menu();
        break;
    default:
        break;
    }
}

void GameScene::settings_manager_set() {
    m_timer_widget->set_show_millis(m_settings_manager->get_settings().show_milliseconds);
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

void GameScene::handle_win_lost() {
    const bool is_game_lost = m_board_widget->get_board().is_lost();
    const bool is_game_won = m_board_widget->get_board().is_won();

    if (m_game_finished || (!is_game_won && !is_game_lost)) {
        return;
    }
    m_timer_widget->stop();
    m_game_finished = true;

    const std::u16string time = m_timer_widget->build_widget(m_timer_widget->get_minimum_size()).get_canvas_element();

    std::function<void()> return_to_menu = [this]() {
        request_scene_change_with_transition(std::make_unique<MainSelectionScene>());
    };

    std::function<void()> view_board = [this]() {
        pop_dialogue();
    };

    std::shared_ptr<Widget> main_dialogue;
    if (is_game_won) {
        main_dialogue = std::make_shared<RainbowSwitcher>(
            std::make_shared<CustomDrawer>(u"You Won!\nIt took " + time, u'\n'),
            get_all_colors_except_black(), true);
    } else {
        main_dialogue = std::make_shared<CustomDrawer>(u"You lost after " + time, u'\n');
    }


    std::shared_ptr<InformDialogue> dialogue_widget = std::make_shared<InformDialogue>(
        main_dialogue, u"Return to Menu", u"View Board", return_to_menu, view_board);

    if (is_game_won) {
        const std::shared_ptr<InputDialogue> input_widget = std::make_shared<InputDialogue>(
            m_keyboard_controller,
            [this](const std::string& text) {
                const ScoreBoardEntry score_board_entry{text, m_timer_widget->get_time_in_millis()};
                m_score_board_manager->add_entry(m_size_key, m_difficulty_key, score_board_entry);
                pop_dialogue();
            }, u"Name: ", MAX_NAME_LENGTH);
        const std::shared_ptr<Dialogue> input_dialogue = InputDialogue::get_dialogue(input_widget);

        input_dialogue->set_on_dismiss([dialogue_widget]() {
            dialogue_widget->unselect();
        });

        std::shared_ptr<Dialogue> already_dialogue = SimpleDialogue::get_dialogue(
            std::make_shared<CustomDrawer>(u"You already saved to scoreboard!"));

        already_dialogue->set_on_dismiss([dialogue_widget]() {
            dialogue_widget->unselect();
        });

        dialogue_widget->add_options(u"Save to Scoreboard", [this, input_dialogue, input_widget, already_dialogue]() {
            if (!m_score_already_saved) {
                show_dialogue(input_dialogue, InputDialogue::get_stack_info());
                input_widget->activate_dialogue();
                m_score_already_saved = true;
            } else {
                show_dialogue(already_dialogue, SimpleDialogue::get_stack_info());
            }
        });
    }

    DialogueOptions dialogue_options = InformDialogue::get_dialogue_options();
    dialogue_options.update_background = true;

    const std::shared_ptr<Dialogue> dialogue = std::make_shared<Dialogue>(dialogue_widget, dialogue_options);
    dialogue->set_on_dismiss([this]() {
        m_game_column->push_child(std::make_shared<Alignment>(
            std::make_shared<CustomDrawer>(u"Press 'enter' to return to the menu!"),
            MIDDLE_CENTER));
        set_dirty();
    });
    show_dialogue(dialogue, InformDialogue::get_stack_info());
}

void GameScene::open_pause_menu() {
    std::function<void()> return_to_menu = [this]() {
        request_scene_change_with_transition(std::make_unique<MainSelectionScene>());
    };

    const std::function<void()> back_to_board = [this]() {
        pop_dialogue();
    };

    std::function<void()> open_controls_dialogue = [this]() {
        show_dialogue(ControlsDialogue::get_dialogue(), ControlsDialogue::get_stack_info());
    };

    std::shared_ptr<Widget> main_dialogue = std::make_shared<CustomDrawer>(u"Game Paused!", u'\n');
    std::shared_ptr<InformDialogue> pause_widget = std::make_shared<InformDialogue>(
        main_dialogue, u"Controls", u"Return to Menu", open_controls_dialogue, return_to_menu);

    pause_widget->add_options(u"Continue", back_to_board);
    pause_widget->set_selectable(false);

    const std::shared_ptr<Dialogue> pause_dialogue = std::make_shared<Dialogue>(
        pause_widget, InformDialogue::get_dialogue_options());
    show_dialogue(pause_dialogue, InformDialogue::get_stack_info());
}
