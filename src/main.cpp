#include "api/controller/terminal_controller.hpp"
#include "api/ui/scene/main_selection_scene.hpp"


int main(int argc, char *argv[]) {
    // std::shared_ptr<BoardWidget> board_widget = std::make_shared<BoardWidget>(std::make_shared<Board2D>(Vector2D{30, 20}, 5, false));

    // TerminalController controller{std::make_unique<GameScene>(board_widget)};
    TerminalController controller{std::make_unique<MainSelectionScene>()};
    controller.run();
}
