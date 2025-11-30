#include "test_scene.hpp"

#include "api/helper/file_reader.hpp"
#include "../canvas/terminal_helper.hpp"
#include "../widget/widgets/banner_widget.hpp"
#include "../widget/reactive_widgets/text_selection_widget.hpp"
#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/column.hpp"
#include "api/ui/widget/widgets/row.hpp"

#include <vector>

TestScene::TestScene() {
	const FileReader fileReader("assets/banner.txt");
	std::string content;
	fileReader.read_string_content(content);

	std::unique_ptr<Alignment> aligned_banner = std::make_unique<Alignment>(
		Alignment(std::make_unique<BannerWidget>(CanvasElement(content)), MIDDLE_CENTER));
	aligned_banner->m_flex = 1;

	std::unique_ptr<TextSelectionWidget> selection = std::make_unique<TextSelectionWidget>(true, true);
	selection->add_option("Beginner", []() {
		show_temporary_message("Spielfeld von 8 mal 8 (64) Feldern mit 10 Minen (Minendichte 15,6 %)");
	});
	selection->add_option("Advanced", []() {
		show_temporary_message("Spielfeld von 16 mal 16 (256) Feldern mit 40 Minen (15,6 %)");
	});
	selection->add_option("Professional", []() {
		show_temporary_message("Spielfeld von 30 mal 16 (480) Feldern mit 99 Minen (20,6 %)");
	});
	selection->add_option("Exit", []() {
	});
	selection->set_selected_index(0);

	std::unique_ptr<Alignment> aligned_selection = std::make_unique<Alignment>(std::move(selection), BOTTOM_LEFT);
	aligned_selection->m_flex = NO_FLEX;

	std::vector<std::unique_ptr<Widget> > children;
	children.push_back(std::move(aligned_banner));
	children.push_back(std::move(aligned_selection));
	m_base_widget = std::make_unique<Column>(std::move(children));
}