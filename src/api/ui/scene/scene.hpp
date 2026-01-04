#pragma once
#include <algorithm>

#include "api/ui/widget/widget.hpp"
#include "dialogue.hpp"
#include <memory>
#include <vector>
#include <functional>

#include "api/ui/widget/widgets/stack.hpp"

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

    void request_exit() {
        m_exit_requested = true;
    }

    bool is_exit_requested() const {
        return m_exit_requested;
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
        for (const std::shared_ptr<Dialogue> &dialogue: m_dialogue_stack) {
            dialogue->get_widget()->set_dirty();
        }
    }

    virtual bool is_dirty() {
        if (m_base_widget->is_dirty()) {
            return true;
        }
        return std::ranges::any_of(m_dialogue_stack.begin(), m_dialogue_stack.end(), [](const std::shared_ptr<Dialogue> &dialogue) {
            return dialogue->get_widget()->is_dirty();
        });
    }

    void keyboard_press(const int key) {
        if (!m_dialogue_stack.empty()) {
            const std::shared_ptr<Dialogue> &dialogue = m_dialogue_stack.back();
            if (key == 27 && dialogue->get_options().dismissible) {
                pop_dialogue();
                return;
            }
            dialogue->get_widget()->keyboard_press(key);
            return;
        }
        handle_key(key);
    }

    void update(const double delta_time) {
        std::vector<std::shared_ptr<Dialogue> > to_remove;

        bool further_update = true;
        for (int i = static_cast<int>(m_dialogue_stack.size()) - 1; i >= 0; i--) {
            const std::shared_ptr<Dialogue> &dialogue = m_dialogue_stack[i];

            if (dialogue->is_close_transition_finished()) {
                dialogue->trigger_dismiss();
                m_dialogue_stack.erase(std::next(m_dialogue_stack.begin(), i));
                m_dialogue_stack_widget->pop_widget(m_dialogue_stack_widget->get_stacked_count() - 1);
                set_dirty();
                continue;
            }

            if (!further_update) {
                continue;
            }

            dialogue->get_widget()->update(delta_time);
            further_update = dialogue->get_options().update_background;
        }

        if (further_update) {
            handle_update(delta_time);
        }
    }

    virtual const CanvasElement &build_scene(const Vector2D &size) {
        ensure_stack_initialized();
        return m_dialogue_stack_widget->build_widget(size);
    }

    const std::shared_ptr<Widget> &get_base_widget() {
        return m_base_widget;
    }

    void show_dialogue(std::shared_ptr<Dialogue> dialogue, const StackInfo &info = StackInfo()) {
        ensure_stack_initialized();
        m_dialogue_stack_widget->push_new_widget(dialogue->get_widget(), info);
        m_dialogue_stack.push_back(std::move(dialogue));

        set_dirty();
    }

    void pop_dialogue() {
        if (!m_dialogue_stack.empty()) {
            m_dialogue_stack.back()->start_close_transition();
            set_dirty();
        }
    }

    bool has_active_dialogue() const {
        return !m_dialogue_stack.empty();
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
    bool m_exit_requested = false;

private:
    std::vector<std::shared_ptr<Dialogue> > m_dialogue_stack;
    std::shared_ptr<Stack> m_dialogue_stack_widget;

    void ensure_stack_initialized() {
        if (!m_dialogue_stack_widget && m_base_widget) {
            m_dialogue_stack_widget = std::make_shared<Stack>(m_base_widget, StackInfo());
        }
    }
};
