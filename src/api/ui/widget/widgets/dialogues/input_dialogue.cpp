#include "input_dialogue.hpp"

#include "../alignment.hpp"
#include "../padding.hpp"
#include "../border/border.hpp"
#include "api/ui/widget/widgets/text_input.hpp"

InputDialogue::InputDialogue(const std::shared_ptr<KeyboardController>& keyboard_controller,
                             std::function<void(std::string)> string_callback, const std::u16string& suffix,
                             const int max_length) {
    m_text_input = std::make_shared<TextInput>(std::move(string_callback), suffix, max_length);
    m_text_input->set_keyboard_controller(keyboard_controller);

    std::shared_ptr<Widget> bordered_input = std::make_shared<Border>(
        std::make_shared<Padding>(m_text_input, 2, 2, 1, 1),
        BorderStyle::single_line_border()
    );
    const std::shared_ptr<Widget> aligned_text_input = std::make_shared<Alignment>(bordered_input, MIDDLE_CENTER);


    m_contructed_widget = std::make_shared<Border>(std::make_shared<Padding>(aligned_text_input, 3, 3, 1, 1),
                                                   BorderStyle::double_line_border());
}

void InputDialogue::activate_dialogue() const {
    m_text_input->on_activate();
}

Vector2D InputDialogue::get_minimum_size() const {
    return m_contructed_widget->get_minimum_size();
}

void InputDialogue::keyboard_press(const int key) {
    m_contructed_widget->keyboard_press(key);
}

void InputDialogue::update(const double delta_time) {
    m_contructed_widget->update(delta_time);
}

bool InputDialogue::is_dirty() const {
    return m_is_dirty || m_contructed_widget->is_dirty();
}

CanvasElement InputDialogue::build_canvas_element(const Vector2D& size) {
    return m_contructed_widget->build_widget(size);
}

DialogueOptions InputDialogue::get_dialogue_options() {
    return {};
}

StackInfo InputDialogue::get_stack_info() {
    StackInfo stack_info;
    stack_info.height_percentage = 0.4;
    stack_info.width_percentage = 0.3;
    return stack_info;
}

std::shared_ptr<Dialogue> InputDialogue::get_dialogue(const std::shared_ptr<InputDialogue>& dialogue) {
    return std::make_shared<Dialogue>(dialogue, get_dialogue_options());
}
