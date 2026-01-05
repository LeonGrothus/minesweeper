#pragma once

#include "scene.hpp"

#include "../widget/widgets/selection_widget.hpp"
#include "api/ui/widget/widgets/visibility.hpp"
#include "api/ui/widget/widgets/alignment.hpp"

#include <memory>

#include "api/ui/widget/widgets/transition_widget.hpp"
#include "api/ui/widget/widgets/boards/board_widget.hpp"

class MainSelectionScene final : public Scene {
public:
    MainSelectionScene();

    void handle_key(int key) override;

    void handle_update(double delta_time) override;

private:
    enum class Stage {
        Main,
        Size,
        Difficulty,
        Confirm
    };

    enum class Size {
        Small,
        Medium,
        Large
    };

    enum class Difficulty {
        NoDifficulty,
        VeryEasy,
        Easy,
        Medium,
        Hard
    };

    Stage m_stage = Stage::Main;
    Size m_selected_size = Size::Small;
    Difficulty m_selected_difficulty = Difficulty::NoDifficulty;

    std::shared_ptr<Alignment> m_aligned_banner_widget;
    std::shared_ptr<TransitionWidget> m_display_widget;

    std::shared_ptr<SelectionWidget> m_main_menu;
    std::shared_ptr<SelectionWidget> m_size_menu;
    std::shared_ptr<SelectionWidget> m_difficulty_menu;
    std::shared_ptr<SelectionWidget> m_confirm_menu;

    std::shared_ptr<Visibility> m_size_visible;
    std::shared_ptr<Visibility> m_difficulty_visible;
    std::shared_ptr<Visibility> m_confirm_visible;

    std::shared_ptr<SelectionWidget> active_menu() const;

    void go_to_stage(Stage stage);

    void update_display_widget() const;

    static std::shared_ptr<Widget> create_board_showcase(Size size, Difficulty difficulty);

    static std::shared_ptr<BoardWidget> create_board(Size size, Difficulty difficulty);

    static Vector2D get_board_size(Size size);

    static float get_mine_percentage(Difficulty difficulty);

    static std::shared_ptr<Alignment> wrap_with_alignment(const std::shared_ptr<Widget> &widget);
};
