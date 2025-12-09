#pragma once

#include "scene.hpp"

#include "api/ui/widget/reactive_widgets/text_selection_widget.hpp"
#include "api/ui/widget/widgets/visibility.hpp"
#include "api/ui/widget/widgets/alignment.hpp"

#include <memory>

#include "api/ui/widget/widgets/banner_widget.hpp"
#include "api/ui/widget/widgets/board_showcase_widget.hpp"

class MainSelectionScene final : public Scene {
public:
    MainSelectionScene();

    void keyboard_press(int key) override;

    void update(double delta_time) override;

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
        Easy,
        Medium,
        Hard
    };

    Stage m_stage = Stage::Main;
    Size m_selected_size = Size::Small;
    Difficulty m_selected_difficulty = Difficulty::NoDifficulty;

    std::shared_ptr<BannerWidget> m_banner_widget;
    std::shared_ptr<Widget> m_display_widget;
    std::shared_ptr<Alignment> m_display_alignment;

    std::shared_ptr<TextSelectionWidget> m_main_menu;
    std::shared_ptr<TextSelectionWidget> m_size_menu;
    std::shared_ptr<TextSelectionWidget> m_difficulty_menu;
    std::shared_ptr<TextSelectionWidget> m_confirm_menu;

    std::shared_ptr<Visibility> m_size_visible;
    std::shared_ptr<Visibility> m_difficulty_visible;
    std::shared_ptr<Visibility> m_confirm_visible;

    std::shared_ptr<TextSelectionWidget> active_menu() const;

    void go_to_stage(Stage stage);

    void update_display_widget();

    static std::shared_ptr<BoardShowcaseWidget> create_board_showcase(Size size, Difficulty difficulty);

    static Vector2D get_board_size(Size size);

    static float get_mine_percentage(Difficulty difficulty);
};
