#include "api/controller/terminal_controller.hpp"
#include "api/ui/scene/test_scene.hpp"


int main(int argc, char *argv[]) {
	TerminalController controller{std::make_unique<TestScene>()};
	controller.run();
}
