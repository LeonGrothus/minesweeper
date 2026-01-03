#pragma once
#include "scene.hpp"
#include "api/ui/widget/widgets/transition_widget.hpp"

class TransitionScene : public Scene {
public:
    explicit TransitionScene(const std::unique_ptr<Scene> &start_scene, std::unique_ptr<Scene> end_scene);

    explicit TransitionScene(const std::shared_ptr<Widget> &start_widget, std::unique_ptr<Scene> end_scene);

    void handle_update(double delta_time) override;

private:
    std::shared_ptr<TransitionWidget> m_transition_widget;
    std::unique_ptr<Scene> m_end_scene;
};
