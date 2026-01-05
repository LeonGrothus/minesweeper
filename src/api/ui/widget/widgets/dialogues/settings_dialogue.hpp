#pragma once
#include <memory>

#include "api/controller/settings_manager.hpp"
#include "api/ui/widget/widget.hpp"

class SettingsDialogue : public Widget {
public:
    explicit SettingsDialogue(const std::shared_ptr<SettingsManager> &settings_manager);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Widget> m_contructed_widget;
};
