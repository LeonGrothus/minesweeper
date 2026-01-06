#include "credits_dialogue.hpp"

#include "../../../../controller/color_manager.hpp"
#include "../custom_drawer.hpp"
#include "../alignment.hpp"
#include "../padding.hpp"
#include "../border/border.hpp"
#include "../rainbow_switcher.hpp"
#include "api/helper/conversion_helper.hpp"
#include "api/helper/file_manager.hpp"
#include "api/ui/widget/widgets/banner_widget.hpp"
#include "api/ui/widget/widgets/Rotation.hpp"

CreditsDialogue::CreditsDialogue() {
    const std::shared_ptr<RainbowSwitcher> credits_rainbow = std::make_shared<RainbowSwitcher>(
        std::make_shared<BannerWidget>("assets/credits.txt"), get_all_colors_except_black(), true);

    const std::shared_ptr<Rotation> rotation_text = std::make_shared<Rotation>(credits_rainbow, -6, 6);
    rotation_text->set_loop_time(10000);
    const std::shared_ptr<Widget> credits_text = std::make_shared<Alignment>(rotation_text, MIDDLE_CENTER);

    m_contructed_widget = std::make_shared<Border>(std::make_shared<Padding>(credits_text, 3, 3, 1, 1), BorderStyle::double_line_border());
}

Vector2D CreditsDialogue::get_minimum_size() const {
    return m_contructed_widget->get_minimum_size();
}

void CreditsDialogue::keyboard_press(const int key) {
    m_contructed_widget->keyboard_press(key);
}

void CreditsDialogue::update(const double delta_time) {
    m_contructed_widget->update(delta_time);
}

bool CreditsDialogue::is_dirty() const {
    return m_is_dirty || m_contructed_widget->is_dirty();
}

CanvasElement CreditsDialogue::build_canvas_element(const Vector2D &size) {
    return m_contructed_widget->build_widget(size);
}

DialogueOptions CreditsDialogue::getDialogueOptions() {
    return {};
}

StackInfo CreditsDialogue::getStackInfo() {
    StackInfo stack_info;
    stack_info.height_percentage = 0.4;
    stack_info.width_percentage = 0.3;
    return stack_info;
}

std::shared_ptr<Dialogue> CreditsDialogue::getDialogue() {
    return std::make_shared<Dialogue>(std::make_shared<CreditsDialogue>(), getDialogueOptions());
}
