#include "test_scene.hpp"

#include "api/helper/file_reader.hpp"
#include "api/ui/terminal_helper.hpp"
#include "widgets/banner_widget.hpp"
#include "widgets/text_selection_widget.hpp"

#include <memory>
#include <vector>

TestScene::TestScene() {
	FileReader fileReader("assets/banner.txt");
	std::string content;
	fileReader.read_string_content(content);

	std::unique_ptr<BannerWidget> banner = std::make_unique<BannerWidget>(CanvasElement(content), MIDDLE_CENTER);
	banner->m_flex = 1;

	std::unique_ptr<TextSelectionWidget> selection = std::make_unique<TextSelectionWidget>(true);
	selection->add_option("Beginner", []() {
		show_temporary_message("Spielfeld von 8 mal 8 (64) Feldern mit 10 Minen (Minendichte 15,6 %)");
	});
	selection->add_option("Advanced", []() {
		show_temporary_message("Spielfeld von 16 mal 16 (256) Feldern mit 40 Minen (15,6 %)");
	});
	selection->add_option("Professional", []() {
		show_temporary_message("Spielfeld von 30 mal 16 (480) Feldern mit 99 Minen (20,6 %)");
	});
	selection->add_option("Exit", [this]() {
		m_running = false;
	});
	selection->set_selected_index(0);
	selection->m_flex = NO_FLEX;

	std::vector<std::unique_ptr<Widget> > children;
	children.push_back(std::move(banner));
	children.push_back(std::move(selection));
	m_column = std::make_unique<Column>(std::move(children));
}

bool TestScene::is_dirty() {
	return m_column->is_dirty();
}

void TestScene::keyboard_press(const int key) {
	m_column->keyboard_press(key);
}

bool TestScene::update(const double delta_time) {
	m_column->update(delta_time);
	return m_running;
}

void TestScene::draw() {
	int width;
	int height;
	get_terminal_size(width, height);

	ElementSize size{width, height};
	const CanvasElement canvas = m_column->build_widget(size);

	render_to_ncurses(canvas.get_canvas_element(), canvas.get_element_size());
}