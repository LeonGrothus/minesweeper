#include "game_scene.hpp"

#include "api/ui/widget/widgets/alignment.hpp"

GameScene::GameScene(const std::shared_ptr<BoardWidget> &to_play) : m_board_widget(to_play) {
    const std::shared_ptr<Alignment> aligned_board = std::make_shared<Alignment>(to_play, MIDDLE_CENTER);
    m_base_widget = aligned_board;
}
