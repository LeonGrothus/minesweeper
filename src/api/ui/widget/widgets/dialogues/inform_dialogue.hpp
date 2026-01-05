#pragma once
#include <functional>
#include <memory>

#include "api/ui/widget/widget.hpp"
#include "../selection_widget.hpp"
#include "api/ui/scene/dialogue.hpp"
#include "api/ui/widget/widgets/stack.hpp"

class InformDialogue : public Widget {
public:
    explicit InformDialogue(std::shared_ptr<Widget> main_widget, const std::u16string &left_text, const std::u16string &right_text,
                            const std::function<void()> &left_callback, const std::function<void()> &right_callback);

    void add_options(const std::u16string &option, const std::function<void()> &callback);

    void set_selectable(bool selectable) const;

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

    static DialogueOptions getDialogueOptions();

    static StackInfo getStackInfo();

    static std::shared_ptr<Dialogue> getDialogue(const std::shared_ptr<InformDialogue> &widget);

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Widget> m_contructed_widget;

    std::shared_ptr<SelectionWidget> m_selection;
};
