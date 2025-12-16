#pragma once
#include <memory>
#include <vector>

#include "api/ui/widget/widget.hpp"

class TransitionWidget : public Widget {
public:
    explicit TransitionWidget(const std::shared_ptr<Widget> &end, bool fade_in);

    explicit TransitionWidget(const std::shared_ptr<Widget> &start, const std::shared_ptr<Widget> &end);

    void set_new_end(const std::shared_ptr<Widget> &new_end);

    void set_transition_time(float transition_time);

    void set_transition_char(char16_t transition_char);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    void init_transition(Vector2D size);

    void handle_next_transitions(int count);

    double m_transition_time = 600; //ms
    double m_char_reveal_time = 0;

    double m_passed_time = 0;

    char16_t m_transition_char = u'#';

    bool m_transition_finished = false;
    bool m_set_new_end = false;
    bool m_break_between_transition = false;

    int m_to_change = 0;
    std::vector<int> m_cover_indices = std::vector<int>();
    std::vector<int> m_uncover_indices = std::vector<int>();

    std::shared_ptr<Widget> m_start_widget;
    std::shared_ptr<Widget> m_end_widget;

    Vector2D m_canvas_size = Vector2D{0, 0};

    CanvasElement m_start_canvas;
    CanvasElement m_end_canvas;
};
