#pragma once
#include <memory>

#include "api/controller/settings_manager.hpp"
#include "api/ui/scene/dialogue.hpp"
#include "api/ui/widget/widget.hpp"
#include "api/ui/widget/widgets/selection_widget.hpp"
#include "api/ui/widget/widgets/stack.hpp"
#include "api/ui/widget/widgets/settings/list_setting.hpp"


class Dialogue;

class SettingsDialogue : public Widget {
public:
    explicit SettingsDialogue(const std::shared_ptr<SettingsManager> &settings_manager);

    void add_custom_option(const std::shared_ptr<ListSetting> &option) const;

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

    static DialogueOptions getDialogueOptions();

    static StackInfo getStackInfo();

    static std::shared_ptr<Dialogue> getDialogue(const std::shared_ptr<SettingsManager> &settings_manager);

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<SelectionWidget> m_selection_widget;
    std::shared_ptr<Widget> m_contructed_widget;
};
