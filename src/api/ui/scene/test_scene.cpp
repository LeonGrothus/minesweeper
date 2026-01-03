#include "test_scene.hpp"


#include <memory>

#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/custom_drawer.hpp"
#include "api/ui/widget/widgets/timer.hpp"
#include "api/ui/widget/widgets/boards/board_2d_widget.hpp"
#include "api/ui/widget/widgets/boards/board_widget.hpp"

class Border;

TestScene::TestScene() {
    // std::shared_ptr<Board2dWidget> board_widget = std::make_shared<Board2dWidget>(std::make_shared<Board2D>(Vector2D{30, 16}, 30, false));
    // board_widget->set_x_spacing(2);
    // std::shared_ptr<Padding> padded_board_widget = std::make_shared<Padding>(board_widget, 1, 1, 0, 0);
    // std::shared_ptr<Border> bordered_board_widget = std::make_shared<Border>(padded_board_widget, BorderStyle::double_line_border());
    //
    // std::shared_ptr<EnumerationWidget> enumerated_widget = std::make_shared<EnumerationWidget>(bordered_board_widget, 1, 2, 0);
    //
    // m_base_widget = std::make_shared<Border>(std::make_shared<Alignment>(enumerated_widget, MIDDLE_CENTER),
    //                                          BorderStyle::double_line_border());

    // const std::shared_ptr<Alignment> board_widget = std::make_shared<Alignment>(std::make_shared<Timer>(true, true), MIDDLE_CENTER);

    m_base_widget = std::make_shared<Empty>();

    const std::shared_ptr<Widget> settings_widget = std::make_shared<Border>(
        std::make_shared<Padding>(std::make_shared<Empty>(Vector2D{4, 4}, u'#'), 1),
        BorderStyle::double_line_border());
    DialogueOptions options;
    options.use_transition = true;
    StackInfo info;
    info.height_percentage = 0.5;
    info.width_percentage = 0.5;
    info.absolute_size = settings_widget->get_minimum_size();
    show_dialogue(settings_widget, options, info);
}
