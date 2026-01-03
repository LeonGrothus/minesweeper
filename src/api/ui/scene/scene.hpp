#pragma once
#include "api/ui/widget/widget.hpp"
#include <memory>

class Scene {
public:
    virtual ~Scene() = default;

    void request_scene_change(std::unique_ptr<Scene> next_scene) {
        m_pending_scene = std::move(next_scene);
        m_use_transition = false;
    }

    void request_scene_change_with_transition(std::unique_ptr<Scene> next_scene) {
        m_pending_scene = std::move(next_scene);
        m_use_transition = true;
    }

    std::unique_ptr<Scene> take_pending_scene() {
        return std::move(m_pending_scene);
    }

    bool should_use_transition() const {
        return m_use_transition;
    }

    virtual void set_dirty() {
        if (m_base_widget) {
            m_base_widget->set_dirty();
        }
    }

    virtual bool is_dirty() {
        return m_base_widget->is_dirty();
    }

    virtual void keyboard_press(const int key) {
        m_base_widget->keyboard_press(key);
    }

    virtual void update(const double delta_time) {
        m_base_widget->update(delta_time);
    }

    virtual const CanvasElement &build_scene(const Vector2D &size) {
        return m_base_widget->build_widget(size);
    }

    const std::shared_ptr<Widget> &get_base_widget() {
        return m_base_widget;
    }

protected:
    virtual void handle_key(const int key) {
        m_base_widget->keyboard_press(key);
    }

    virtual void handle_update(const double delta_time) {
        m_base_widget->update(delta_time);
    }

    std::shared_ptr<Widget> m_base_widget;
    std::unique_ptr<Scene> m_pending_scene;
    bool m_use_transition = false;
};

inline Scene::~Scene() = default;
