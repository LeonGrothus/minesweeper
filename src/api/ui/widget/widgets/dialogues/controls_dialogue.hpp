#pragma once
#include <memory>

#include "api/ui/scene/dialogue.hpp"
#include "api/ui/widget/widget.hpp"
#include "api/ui/widget/widgets/stack.hpp"

class ControlsDialogue : public Widget {
public:
    ControlsDialogue();

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

    static DialogueOptions getDialogueOptions();

    static StackInfo getStackInfo();

    static std::shared_ptr<Dialogue> getDialogue();

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Widget> m_contructed_widget;
};
