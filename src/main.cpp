#include "api/helper/file_reader.hpp"
#include "api/ui/canvas_element.hpp"
#include "api/ui/terminal_helper.hpp"
#include <iostream>

void print_banner_to_canvas(CanvasElement &canvas, Position banner_pos) {
	FileReader fileReader("assets/banner.txt");
	std::string content;
	fileReader.read_string_content(content);

	CanvasElement banner(content);

	position_string_on_canvas(banner, banner_pos, canvas);
}

int main(int argc, char *argv[]) {
	int width, height;
	get_terminal_size(width, height);

	CanvasElement canvas(std::string(width * height, ' '), width, height);
	print_banner_to_canvas(canvas, MIDDLE_CENTER);
	print_banner_to_canvas(canvas, TOP_LEFT);

	std::cout << canvas << std::flush;

	return 0;
}
