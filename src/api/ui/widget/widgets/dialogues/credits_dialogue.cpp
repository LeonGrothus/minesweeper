#include "credits_dialogue.hpp"

#include "../../../../controller/color_manager.hpp"
#include "../alignment.hpp"
#include "../rainbow_switcher.hpp"
#include "api/ui/widget/widgets/banner_widget.hpp"
#include "api/ui/widget/widgets/Rotation.hpp"

std::shared_ptr<Widget> make_credits_content() {
    const std::shared_ptr<RainbowSwitcher> credits_rainbow = std::make_shared<RainbowSwitcher>(
        std::make_shared<BannerWidget>("assets/credits.txt"), get_all_colors_except_black(), true);

    const std::shared_ptr<Rotation> rotation_text = std::make_shared<Rotation>(credits_rainbow, -6, 6);
    rotation_text->set_loop_time(10000);
    return std::make_shared<Alignment>(rotation_text, MIDDLE_CENTER);
}

CreditsDialogue::CreditsDialogue() : SimpleDialogue(make_credits_content()) {}

DialogueOptions CreditsDialogue::get_dialogue_options() {
    return {};
}

StackInfo CreditsDialogue::get_stack_info() {
    StackInfo stack_info;
    stack_info.height_percentage = 0.4;
    stack_info.width_percentage = 0.3;
    return stack_info;
}

std::shared_ptr<Dialogue> CreditsDialogue::get_dialogue() {
    return std::make_shared<Dialogue>(std::make_shared<CreditsDialogue>(), get_dialogue_options());
}
