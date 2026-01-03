#include "transition_scene.hpp"

#include "api/ui/widget/widgets/empty.hpp"
#include "api/ui/widget/widgets/transition_widget.hpp"

TransitionScene::TransitionScene(const std::unique_ptr<Scene> &start_scene, std::unique_ptr<Scene> end_scene) : m_end_scene(
    std::move(end_scene)) {
    m_transition_widget = std::make_shared<TransitionWidget>(start_scene->get_base_widget(),
                                                             std::make_unique<Empty>());
    m_base_widget = m_transition_widget;
}

TransitionScene::TransitionScene(const std::shared_ptr<Widget> &start_widget, std::unique_ptr<Scene> end_scene) : m_end_scene(
    std::move(end_scene)) {
    m_transition_widget = std::make_shared<TransitionWidget>(start_widget, m_end_scene->get_base_widget());
    m_base_widget = m_transition_widget;
}

void TransitionScene::handle_update(const double delta_time) {
    if (m_transition_widget->is_transition_finished()) {
        request_scene_change(std::move(m_end_scene));
    }
}
