#include <memory>
#include "api/controller/terminal_controller.hpp"
#include "api/controller/keyboard_controller.hpp"
#include "api/ui/scene/main_selection_scene.hpp"


int main(int argc, char* argv[]) {
    const std::shared_ptr<KeyboardController> keyboard_controller = std::make_shared<KeyboardController>();
    TerminalController controller{keyboard_controller, std::make_unique<MainSelectionScene>()};
    controller.run();
}
