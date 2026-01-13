#pragma once
#include "api/ui/scene/dialogue.hpp"
#include "api/ui/widget/widget.hpp"
#include "api/ui/widget/widgets/stack.hpp"
#include "api/ui/widget/widgets/text_input.hpp"

class InputDialogue : public Widget {
public:
    explicit InputDialogue(const std::shared_ptr<KeyboardController>& keyboard_controller,
                           std::function<void(std::string)> string_callback, const std::u16string& suffix,
                           int max_length);

    void activate_dialogue() const;

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

    static DialogueOptions get_dialogue_options();

    static StackInfo get_stack_info();

    static std::shared_ptr<Dialogue> get_dialogue(const std::shared_ptr<InputDialogue>& dialogue);

protected:
    CanvasElement build_canvas_element(const Vector2D& size) override;

private:
    std::shared_ptr<Widget> m_contructed_widget;
    std::shared_ptr<TextInput> m_text_input;
};
