#pragma once
#include "scene.hpp"
#include "../widget/widgets/boards/board_widget.hpp"

class GameScene : public Scene {
public:
    explicit GameScene(const std::shared_ptr<BoardWidget> &to_play);

private:
    std::shared_ptr<BoardWidget> m_board_widget;
};
