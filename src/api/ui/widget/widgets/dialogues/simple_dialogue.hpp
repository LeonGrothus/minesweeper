#pragma once

#pragma once
#include <memory>

#include "api/ui/scene/dialogue.hpp"
#include "api/ui/widget/widget.hpp"
#include "api/ui/widget/widgets/stack.hpp"

class SimpleDialogue : public Widget {
public:
    explicit SimpleDialogue(const std::shared_ptr<Widget>& to_display);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

    static DialogueOptions get_dialogue_options();

    static StackInfo get_stack_info();

    static std::shared_ptr<Dialogue> get_dialogue(const std::shared_ptr<Widget>& to_display);

protected:
    CanvasElement build_canvas_element(const Vector2D& size) override;

private:
    std::shared_ptr<Widget> m_contructed_widget;
};
