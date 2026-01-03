#include "dialogue.hpp"

#include "api/controller/terminal_controller.hpp"
#include "api/ui/widget/widgets/empty.hpp"

Dialogue::Dialogue(std::shared_ptr<Widget> content, const DialogueOptions &options) : m_options(options), m_content(std::move(content)) {
    if (m_options.use_transition) {
        m_transition_widget = std::make_shared<TransitionWidget>(std::make_shared<Empty>(Vector2D{}, ALPHA_CHAR), m_content);
        m_transition_widget->set_transition_time(m_options.transition_time);
    }
}

std::shared_ptr<Widget> Dialogue::get_widget() const {
    if (m_options.use_transition) {
        return m_transition_widget;
    }
    return m_content;
}

bool Dialogue::is_open_transition_finished() const {
    if (!m_options.use_transition || !m_transition_widget) {
        return true;
    }
    if (m_closing) {
        return false;
    }
    return m_transition_widget->is_transition_finished();
}

void Dialogue::start_close_transition() {
    m_closing = true;
    if (m_options.use_transition && m_transition_widget) {
        m_transition_widget->set_new_end(std::make_shared<Empty>(Vector2D{1, 1}, ALPHA_CHAR));
    }
}

bool Dialogue::is_close_transition_finished() const {
    if (!m_options.use_transition || !m_transition_widget) {
        return true;
    }
    if (!m_closing) {
        return false;
    }
    return m_transition_widget->is_transition_finished();
}

const DialogueOptions &Dialogue::get_options() const {
    return m_options;
}

void Dialogue::set_on_dismiss(std::function<void()> callback) {
    m_on_dismiss = std::move(callback);
}

void Dialogue::trigger_dismiss() const {
    if (m_on_dismiss) {
        m_on_dismiss();
    }
}
