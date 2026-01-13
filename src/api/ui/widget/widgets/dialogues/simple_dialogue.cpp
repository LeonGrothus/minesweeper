#include "simple_dialogue.hpp"

#include "credits_dialogue.hpp"

#include "../../../../controller/color_manager.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/border/border.hpp"

SimpleDialogue::SimpleDialogue(const std::shared_ptr<Widget>& to_display, const bool warp_with_align_border) {
    if (!warp_with_align_border) {
        m_contructed_widget = to_display;
        return;
    }

    const std::shared_ptr<Widget> aligned_display = std::make_shared<Alignment>(to_display, MIDDLE_CENTER);

    m_contructed_widget = std::make_shared<Border>(std::make_shared<Padding>(aligned_display, 3, 3, 1, 1),
                                                   BorderStyle::double_line_border());
}

Vector2D SimpleDialogue::get_minimum_size() const {
    return m_contructed_widget->get_minimum_size();
}

void SimpleDialogue::keyboard_press(const int key) {
    m_contructed_widget->keyboard_press(key);
}

void SimpleDialogue::update(const double delta_time) {
    m_contructed_widget->update(delta_time);
}

bool SimpleDialogue::is_dirty() const {
    return m_is_dirty || m_contructed_widget->is_dirty();
}

CanvasElement SimpleDialogue::build_canvas_element(const Vector2D& size) {
    return m_contructed_widget->build_widget(size);
}

DialogueOptions SimpleDialogue::get_dialogue_options() {
    return {};
}

StackInfo SimpleDialogue::get_stack_info() {
    StackInfo stack_info;
    stack_info.height_percentage = 0.4;
    stack_info.width_percentage = 0.3;
    return stack_info;
}

std::shared_ptr<Dialogue> SimpleDialogue::get_dialogue(const std::shared_ptr<Widget>& to_display) {
    return std::make_shared<Dialogue>(std::make_shared<SimpleDialogue>(to_display), get_dialogue_options());
}
