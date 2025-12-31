#include "main_selection_scene.hpp"

#include <memory>
#include <ncurses.h>
#include <utility>

#include "transition_scene.hpp"
#include "api/helper/file_reader.hpp"
#include "api/ui/canvas/terminal_helper.hpp"
#include "api/ui/scene/game_scene.hpp"
#include "api/ui/widget/widgets/row.hpp"
#include "api/ui/widget/widgets/alignment.hpp"
#include "../widget/widgets/border/border.hpp"
#include "api/ui/widget/widgets/column.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/border/border_style.hpp"
#include "api/game/board/board_2d.hpp"
#include "../widget/widgets/boards/board_widget.hpp"
#include "api/ui/widget/widgets/banner_widget.hpp"
#include "api/ui/widget/widgets/boards/board_showcase_widget.hpp"

MainSelectionScene::MainSelectionScene() {
    FileReader reader("assets/banner.txt");
    std::string content;
    reader.read_string_content(content);
    m_aligned_banner_widget = wrap_with_alignment(std::make_shared<BannerWidget>(content));

    m_display_widget = std::make_shared<TransitionWidget>(m_aligned_banner_widget, true);

    m_main_menu = std::make_shared<TextSelectionWidget>(true, true);
    m_size_menu = std::make_shared<TextSelectionWidget>(true, true);
    m_difficulty_menu = std::make_shared<TextSelectionWidget>(true, true);
    m_confirm_menu = std::make_shared<TextSelectionWidget>(false, true);

    m_main_menu->add_option(u"Play", [this]() {
        go_to_stage(Stage::Size);
    });
    m_main_menu->add_option(u"Settings", [this]() {
        go_to_stage(Stage::Main);
    });
    m_main_menu->add_option(u"Exit", []() {
    });

    m_size_menu->add_option(u"Small", [this]() {
        m_selected_size = Size::Small;
        update_display_widget();
        go_to_stage(Stage::Difficulty);
    });
    m_size_menu->add_option(u"Medium", [this]() {
        m_selected_size = Size::Medium;
        update_display_widget();
        go_to_stage(Stage::Difficulty);
    });
    m_size_menu->add_option(u"Large", [this]() {
        m_selected_size = Size::Large;
        update_display_widget();
        go_to_stage(Stage::Difficulty);
    });

    m_difficulty_menu->add_option(u"Very Easy", [this]() {
        m_selected_difficulty = Difficulty::VeryEasy;
        update_display_widget();
        go_to_stage(Stage::Confirm);
    });
    m_difficulty_menu->add_option(u"Easy", [this]() {
        m_selected_difficulty = Difficulty::Easy;
        update_display_widget();
        go_to_stage(Stage::Confirm);
    });
    m_difficulty_menu->add_option(u"Medium", [this]() {
        m_selected_difficulty = Difficulty::Medium;
        update_display_widget();
        go_to_stage(Stage::Confirm);
    });
    m_difficulty_menu->add_option(u"Hard", [this]() {
        m_selected_difficulty = Difficulty::Hard;
        update_display_widget();
        go_to_stage(Stage::Confirm);
    });

    m_confirm_menu->add_option(u"Play", [this]() {
        std::shared_ptr<BoardWidget> board = create_board(m_selected_size, m_selected_difficulty);
        std::unique_ptr<Scene> game_scene = std::make_unique<GameScene>(board);

        request_scene_change_with_transition(std::move(game_scene));
    });


    std::shared_ptr<Padding> m_main_menu_padded = std::make_shared<Padding>(m_main_menu, 2, 2, 1, 1);
    std::shared_ptr<Padding> m_size_menu_padded = std::make_shared<Padding>(m_size_menu, 2, 2, 1, 1);
    std::shared_ptr<Padding> m_diff_menu_padded = std::make_shared<Padding>(m_difficulty_menu, 2, 2, 1, 1);
    std::shared_ptr<Padding> m_conf_menu_padded = std::make_shared<Padding>(m_confirm_menu, 2, 2, 1, 1);

    const std::shared_ptr<Border> m_main_menu_border = std::make_shared<Border>(m_main_menu_padded,
                                                                                BorderStyle::single_thick_border());
    std::shared_ptr<Border> m_size_menu_border = std::make_shared<Border>(m_size_menu_padded, BorderStyle::single_thick_border());
    std::shared_ptr<Border> m_diff_menu_border = std::make_shared<Border>(m_diff_menu_padded, BorderStyle::single_thick_border());
    std::shared_ptr<Border> m_conf_menu_border = std::make_shared<Border>(m_conf_menu_padded, BorderStyle::single_thick_border());

    m_size_visible = std::make_shared<Visibility>(m_size_menu_border, false);
    m_difficulty_visible = std::make_shared<Visibility>(m_diff_menu_border, false);
    m_confirm_visible = std::make_shared<Visibility>(m_conf_menu_border, false);

    std::vector<std::shared_ptr<Widget> > children;

    children.push_back(m_main_menu_border);
    children.push_back(m_size_visible);
    children.push_back(m_difficulty_visible);
    children.push_back(m_confirm_visible);

    std::shared_ptr<Row> row = std::make_shared<Row>(std::move(children));
    row->set_spacing(3);

    std::shared_ptr<Padding> padded_row = std::make_shared<Padding>(row, 1, 1, 0, 0);
    std::shared_ptr<Widget> selection_widget = std::make_shared<Alignment>(padded_row, BOTTOM_LEFT);
    selection_widget->m_flex = NO_FLEX;

    std::vector<std::shared_ptr<Widget> > layout;
    layout.push_back(m_display_widget);
    layout.push_back(selection_widget);
    m_base_widget = std::make_shared<Border>(std::make_shared<Column>(layout), BorderStyle::double_line_border());

    go_to_stage(Stage::Main);
}

std::shared_ptr<TextSelectionWidget> MainSelectionScene::active_menu() const {
    switch (m_stage) {
        case Stage::Main:
            return m_main_menu;
        case Stage::Size:
            return m_size_menu;
        case Stage::Difficulty:
            return m_difficulty_menu;
        case Stage::Confirm:
            return m_confirm_menu;
    }
    return m_main_menu;
}

void MainSelectionScene::go_to_stage(const Stage stage) {
    m_stage = stage;
    m_size_visible->set_visible(stage >= Stage::Size);
    m_difficulty_visible->set_visible(stage >= Stage::Difficulty);
    m_confirm_visible->set_visible(stage >= Stage::Confirm);

    if (stage == Stage::Main) {
        m_main_menu->unselect();
        m_display_widget->set_new_end(m_aligned_banner_widget);
    }

    if (stage == Stage::Size) {
        m_size_menu->unselect();
    }

    if (stage == Stage::Difficulty) {
        m_difficulty_menu->unselect();
        m_selected_difficulty = Difficulty::NoDifficulty;
        update_display_widget();
    }

    m_base_widget->set_dirty();
    m_main_menu->set_dirty();
    m_size_menu->set_dirty();
    m_difficulty_menu->set_dirty();
    m_confirm_menu->set_dirty();
}

void MainSelectionScene::keyboard_press(const int key) {
    //esc
    if (key == 27 || key == KEY_LEFT) {
        switch (m_stage) {
            case Stage::Main:
                break;
            case Stage::Size:
                go_to_stage(Stage::Main);
                break;
            case Stage::Difficulty:
                go_to_stage(Stage::Size);
                break;
            case Stage::Confirm:
                go_to_stage(Stage::Difficulty);
                break;
        }
        return;
    }

    active_menu()->keyboard_press(key);
}

void MainSelectionScene::update(const double delta_time) {
    m_main_menu->update(delta_time);
    m_size_visible->update(delta_time);
    m_difficulty_visible->update(delta_time);
    m_confirm_visible->update(delta_time);
    m_display_widget->update(delta_time);
}

Vector2D MainSelectionScene::get_board_size(const Size size) {
    switch (size) {
        case Size::Small:
            return Vector2D{8, 8};
        case Size::Medium:
            return Vector2D{16, 16};
        case Size::Large:
            return Vector2D{32, 24};
    }
    return Vector2D{8, 8};
}

float MainSelectionScene::get_mine_percentage(const Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::NoDifficulty:
            return 0;
        case Difficulty::VeryEasy:
            return 0.05;
        case Difficulty::Easy:
            return 0.2;
        case Difficulty::Medium:
            return 0.4;
        case Difficulty::Hard:
            return 0.6;
    }
    return 0;
}

std::shared_ptr<Alignment> MainSelectionScene::wrap_with_alignment(const std::shared_ptr<Widget> &widget) {
    return std::make_shared<Alignment>(widget, MIDDLE_CENTER);
}

std::shared_ptr<Widget> MainSelectionScene::create_board_showcase(const Size size,
                                                                  const Difficulty difficulty) {
    const Vector2D board_size = get_board_size(size);
    float mine_percentage = get_mine_percentage(difficulty);

    std::shared_ptr<Board2D> board = std::make_shared<Board2D>(board_size, mine_percentage, true);
    const std::shared_ptr<BoardShowcaseWidget> board_showcase_widget = std::make_shared<BoardShowcaseWidget>(board);
    board_showcase_widget->set_blinking(mine_percentage != 0);
    return board_showcase_widget;
}

std::shared_ptr<BoardWidget> MainSelectionScene::create_board(const Size size,
                                                              const Difficulty difficulty) {
    const Vector2D board_size = get_board_size(size);
    float mine_percentage = get_mine_percentage(difficulty);

    std::shared_ptr<Board2D> board = std::make_shared<Board2D>(board_size, mine_percentage, false);
    return std::make_shared<BoardWidget>(board);
}


void MainSelectionScene::update_display_widget() const {
    m_display_widget->set_new_end(wrap_with_alignment(create_board_showcase(m_selected_size, m_selected_difficulty)));

    m_display_widget->set_dirty();
    m_base_widget->set_dirty();
}
