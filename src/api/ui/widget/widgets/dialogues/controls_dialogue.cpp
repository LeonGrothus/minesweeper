#include "controls_dialogue.hpp"

#include "../alignment.hpp"
#include "../column.hpp"
#include "../custom_drawer.hpp"
#include "../padding.hpp"
#include <memory>

#include "api/ui/widget/widgets/rainbow_switcher.hpp"
#include "api/ui/widget/widgets/border/border.hpp"

std::shared_ptr<Widget> make_controls_content() {
    const std::shared_ptr<Widget> header = std::make_shared<Alignment>(
        std::make_shared<RainbowSwitcher>(std::make_shared<CustomDrawer>(u"OBJECTIVE"), ColorRole::Mine),
        MIDDLE_CENTER);

    const std::shared_ptr<Widget> objective = std::make_shared<Alignment>(
        std::make_shared<CustomDrawer>(u"Win by flagging all bombs and uncovering all safe fields."), MIDDLE_CENTER);

    const std::shared_ptr<Widget> controls = std::make_shared<Padding>(std::make_shared<Alignment>(
                                                                           std::make_shared<RainbowSwitcher>(
                                                                               std::make_shared<CustomDrawer>(
                                                                                   u"CONTROLS"),
                                                                               ColorRole::Mine), MIDDLE_CENTER), 0, 0,
                                                                       2, 0);

    const std::shared_ptr<Widget> controls_menu = std::make_shared<Alignment>(
        std::make_shared<CustomDrawer>(u"Menus: WASD/Arrows to move, Enter to select, Esc to exit.\n P for Screenshot",
                                       '\n'),
        MIDDLE_CENTER);

    const std::shared_ptr<Widget> controls_game = std::make_shared<Alignment>(
        std::make_shared<CustomDrawer>(u"Game: WASD/Arrows to move, Enter to uncover, F to flag/unflag."),
        MIDDLE_CENTER);

    std::vector<std::shared_ptr<Widget>> column_list{
        header,
        objective,
        controls,
        controls_menu,
        controls_game
    };

    std::shared_ptr<Column> column = std::make_shared<Column>(column_list);
    column->main_axis_alignment(ListAlignment::Start);

    return std::make_shared<Border>(std::make_shared<Padding>(column, 3, 3, 1, 1),
                                    BorderStyle::double_line_border());
}

ControlsDialogue::ControlsDialogue() : SimpleDialogue(make_controls_content(), false) {}

DialogueOptions ControlsDialogue::get_dialogue_options() {
    DialogueOptions options;
    options.update_background = false;
    return options;
}

StackInfo ControlsDialogue::get_stack_info() {
    StackInfo stack_info;
    stack_info.height_percentage = 0.8;
    stack_info.width_percentage = 0.8;
    stack_info.take_focus = true;
    return stack_info;
}

std::shared_ptr<Dialogue> ControlsDialogue::get_dialogue() {
    return std::make_shared<Dialogue>(std::make_shared<ControlsDialogue>(), get_dialogue_options());
}
