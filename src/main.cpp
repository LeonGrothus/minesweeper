#include "api/controller/terminal_controller.hpp"
#include "api/ui/scene/main_selection_scene.hpp"


int main(int argc, char *argv[]) {
    TerminalController controller{std::make_unique<MainSelectionScene>()};
    controller.run();
}
