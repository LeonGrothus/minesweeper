#include "test_scene.hpp"

#include "../canvas/terminal_helper.hpp"
#include "../widget/reactive_widgets/text_selection_widget.hpp"
#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/column.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/row.hpp"
#include "api/ui/widget/widgets/board_widget.hpp"

#include <memory>
#include <vector>

#include "api/ui/widget/widgets/board_showcase_widget.hpp"
#include "api/ui/widget/widgets/border.hpp"

TestScene::TestScene() {
    auto beginner_widget = std::make_shared<BoardShowcaseWidget>(
        std::make_shared<Board2D>(Vector2D{8, 8}, 10, true));
    auto advanced_widget = std::make_shared<BoardShowcaseWidget>(
        std::make_shared<Board2D>(Vector2D{16, 16}, 40, true));
    auto professional_widget = std::make_shared<BoardWidget>(
        std::make_shared<Board2D>(Vector2D{30, 16}, 30, false));

    std::vector<std::shared_ptr<Widget> > board_row_children;

    auto beginner_with_padding = std::make_shared<Padding>(beginner_widget, 4, 4, 2, 2);
    auto advanced_with_padding = std::make_shared<Padding>(advanced_widget, 4, 4, 2, 2);
    auto professional_with_padding = std::make_shared<Padding>(professional_widget, 4, 4, 2, 2);

    board_row_children.push_back(std::make_shared<Alignment>(beginner_with_padding, MIDDLE_CENTER));
    board_row_children.push_back(std::make_shared<Alignment>(advanced_with_padding, MIDDLE_CENTER));
    board_row_children.push_back(std::make_shared<Alignment>(professional_with_padding, MIDDLE_CENTER));

    auto board_row = std::make_shared<Row>(std::move(board_row_children));
    board_row->set_spacing(2);
    auto aligned_boards = std::make_shared<Alignment>(board_row, MIDDLE_CENTER);
    aligned_boards->m_flex = 1;

    auto selection = std::make_shared<TextSelectionWidget>(true, true);
    selection->add_option(u"Beginner", []() {
        show_temporary_message(u"Spielfeld von 8 mal 8 (64) Feldern mit 10 Minen (Minendichte 15,6 %)");
    });
    selection->add_option(u"Advanced", []() {
        show_temporary_message(u"Spielfeld von 16 mal 16 (256) Feldern mit 40 Minen (15,6 %)");
    });
    selection->add_option(u"Professional", []() {
        show_temporary_message(u"Spielfeld von 30 mal 16 (480) Feldern mit 99 Minen (20,6 %)");
    });
    selection->add_option(u"Exit", []() {
    });
    selection->set_selected_index(0);

    auto padding = std::make_shared<Padding>(selection, 6, 6, 2, 2);

    const auto aligned_selection = std::make_shared<Alignment>(padding, BOTTOM_LEFT);
    aligned_selection->m_flex = NO_FLEX;

    std::vector<std::shared_ptr<Widget> > children;
    children.push_back(aligned_boards);
    // children.push_back(aligned_selection);
    auto all_padding = std::make_shared<Padding>(std::make_shared<Column>(std::move(children)), 4,
                                                4, 2, 2);
    auto all_border = std::make_shared<Border>(all_padding, PaddingBorderStyle::double_line_border());
    m_base_widget = all_border;
}
