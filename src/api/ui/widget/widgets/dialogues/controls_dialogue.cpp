#include "controls_dialogue.hpp"

#include "../alignment.hpp"
#include "../column.hpp"
#include "../custom_drawer.hpp"
#include "../padding.hpp"
#include "../border/border.hpp"
#include <memory>

#include "api/ui/widget/widgets/rainbow_switcher.hpp"

ControlsDialogue::ControlsDialogue() {
    const std::shared_ptr<Widget> header = std::make_shared<Alignment>(
        std::make_shared<RainbowSwitcher>(std::make_shared<CustomDrawer>(u"OBJECTIVE"), ColorRole::Mine), MIDDLE_CENTER);

    const std::shared_ptr<Widget> objective = std::make_shared<Alignment>(
        std::make_shared<CustomDrawer>(u"Win by flagging all bombs and uncovering all safe fields."), MIDDLE_CENTER);

    const std::shared_ptr<Widget> controls = std::make_shared<Padding>(std::make_shared<Alignment>(
                                                                           std::make_shared<RainbowSwitcher>(
                                                                               std::make_shared<CustomDrawer>(u"CONTROLS"),
                                                                               ColorRole::Mine), MIDDLE_CENTER), 0, 0, 2, 0);

    const std::shared_ptr<Widget> controls_menu = std::make_shared<Alignment>(
        std::make_shared<CustomDrawer>(u"Menus: WASD/Arrows to move, Enter to select, Esc to exit.\n P for Screenshot", '\n'),
        MIDDLE_CENTER);

    const std::shared_ptr<Widget> controls_game = std::make_shared<Alignment>(
        std::make_shared<CustomDrawer>(u"Game: WASD/Arrows to move, Enter to uncover, F to flag/unflag."), MIDDLE_CENTER);

    std::vector<std::shared_ptr<Widget> > column_list{
        header,
        objective,
        controls,
        controls_menu,
        controls_game
    };

    std::shared_ptr<Column> column = std::make_shared<Column>(column_list);
    column->main_axis_alignment(ListAlignment::Center);

    m_contructed_widget = std::make_shared<Border>(std::make_shared<Padding>(column, 3, 3, 1, 1), BorderStyle::double_line_border());
}

Vector2D ControlsDialogue::get_minimum_size() const {
    return m_contructed_widget->get_minimum_size();
}

void ControlsDialogue::keyboard_press(const int key) {
    m_contructed_widget->keyboard_press(key);
}

void ControlsDialogue::update(const double delta_time) {
    m_contructed_widget->update(delta_time);
}

bool ControlsDialogue::is_dirty() const {
    return m_is_dirty || m_contructed_widget->is_dirty();
}

CanvasElement ControlsDialogue::build_canvas_element(const Vector2D &size) {
    return m_contructed_widget->build_widget(size);
}

DialogueOptions ControlsDialogue::getDialogueOptions() {
    DialogueOptions options;
    options.update_background = false;
    return options;
}

StackInfo ControlsDialogue::getStackInfo() {
    StackInfo stack_info;
    stack_info.height_percentage = 0.8;
    stack_info.width_percentage = 0.8;
    stack_info.take_focus = true;
    return stack_info;
}

std::shared_ptr<Dialogue> ControlsDialogue::getDialogue() {
    return std::make_shared<Dialogue>(std::make_shared<ControlsDialogue>(), getDialogueOptions());
}
