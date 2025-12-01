#include "test_scene.hpp"

#include "../canvas/terminal_helper.hpp"
#include "../widget/reactive_widgets/text_selection_widget.hpp"
#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/column.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/row.hpp"
#include "api/ui/widget/widgets/board_widget.hpp"

#include <vector>

#include "api/ui/widget/widgets/board_showcase_widget.hpp"

TestScene::TestScene() {
    std::unique_ptr<BoardShowcaseWidget> beginner_widget = std::make_unique<BoardShowcaseWidget>(
        std::make_shared<Board2D>(Vector2D{8, 8}, 10, true));
    std::unique_ptr<BoardShowcaseWidget> advanced_widget = std::make_unique<BoardShowcaseWidget>(
        std::make_shared<Board2D>(Vector2D{16, 16}, 40, true));
    std::unique_ptr<BoardShowcaseWidget> professional_widget = std::make_unique<BoardShowcaseWidget>(
        std::make_shared<Board2D>(Vector2D{30, 16}, 99, true));

    std::vector<std::unique_ptr<Widget> > board_row_children;

    std::unique_ptr<Padding> beginner_with_padding = std::make_unique<Padding>(std::move(beginner_widget), 4, 4, 2, 2);
    beginner_with_padding->set_border_char('#');
    std::unique_ptr<Padding> advanced_with_padding = std::make_unique<Padding>(std::move(advanced_widget), 4, 4, 2, 2);
    advanced_with_padding->set_border_char('#');
    std::unique_ptr<Padding> professional_with_padding = std::make_unique<Padding>(std::move(professional_widget), 4, 4, 2, 2);
    professional_with_padding->set_border_char('#');

    board_row_children.push_back(std::make_unique<Alignment>(std::move(beginner_with_padding), MIDDLE_CENTER));
    board_row_children.push_back(std::make_unique<Alignment>(std::move(advanced_with_padding), MIDDLE_CENTER));
    board_row_children.push_back(std::make_unique<Alignment>(std::move(professional_with_padding), MIDDLE_CENTER));

    std::unique_ptr<Row> board_row = std::make_unique<Row>(std::move(board_row_children));
    std::unique_ptr<Alignment> aligned_boards = std::make_unique<Alignment>(std::move(board_row), MIDDLE_CENTER);
    aligned_boards->m_flex = 1;

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

    std::unique_ptr<Padding> padding = std::make_unique<Padding>(std::move(selection), 6, 6, 2, 2);
    padding->set_border_char('#');

    std::unique_ptr<Alignment> aligned_selection = std::make_unique<Alignment>(std::move(padding), BOTTOM_LEFT);
    aligned_selection->m_flex = NO_FLEX;

    std::vector<std::unique_ptr<Widget> > children;
    children.push_back(std::move(aligned_boards));
    children.push_back(std::move(aligned_selection));
    std::unique_ptr<Padding> all_padding = std::make_unique<Padding>(std::make_unique<Column>(std::move(children)), 4,
                                                                     4, 2, 2);
    all_padding->set_border_char('#');
    m_base_widget = std::move(all_padding);
}
