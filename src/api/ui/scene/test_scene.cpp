#include "test_scene.hpp"

#include "../canvas/terminal_helper.hpp"
#include "../widget/reactive_widgets/text_selection_widget.hpp"

#include <memory>
#include <vector>

#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/board_showcase_widget.hpp"
#include "api/ui/widget/widgets/border.hpp"
#include "api/ui/widget/widgets/transition_widget.hpp"

class Border;

TestScene::TestScene() {
    std::shared_ptr<Border> beginner_widget = std::make_shared<Border>(std::make_shared<BoardShowcaseWidget>(
                                                                           std::make_shared<Board2D>(Vector2D{8, 8}, 10, true)),
                                                                       BorderStyle::double_line_border());
    std::shared_ptr<Border> professional_widget = std::make_shared<Border>(std::make_shared<BoardShowcaseWidget>(
                                                                               std::make_shared<Board2D>(Vector2D{30, 16}, 30, true)),
                                                                           BorderStyle::double_line_border());

    const std::shared_ptr<TransitionWidget> transition_widget = std::make_shared<TransitionWidget>(beginner_widget, professional_widget);


    m_base_widget = std::make_shared<Border>(std::make_shared<Alignment>(transition_widget, MIDDLE_CENTER),
                                             BorderStyle::double_line_border());
}
