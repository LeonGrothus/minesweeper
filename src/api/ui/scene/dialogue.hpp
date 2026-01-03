#pragma once

#include <memory>
#include <functional>
#include "api/ui/widget/widget.hpp"
#include "api/ui/widget/widgets/transition_widget.hpp"

struct DialogueOptions {
    //update background while open
    bool update_background = true;

    bool use_transition = true;

    double transition_time = 500; //ms

    //should close with esc key
    bool dismissible = true;
};


class Dialogue {
public:
    explicit Dialogue(std::shared_ptr<Widget> content, const DialogueOptions &options = DialogueOptions());

    std::shared_ptr<Widget> get_widget() const;

    bool is_open_transition_finished() const;

    void start_close_transition();

    bool is_close_transition_finished() const;

    const DialogueOptions &get_options() const;

    void set_on_dismiss(std::function<void()> callback);

    void trigger_dismiss() const;

private:
    DialogueOptions m_options;

    std::shared_ptr<Widget> m_content;
    std::shared_ptr<TransitionWidget> m_transition_widget;

    bool m_closing = false;
    std::function<void()> m_on_dismiss;
};
