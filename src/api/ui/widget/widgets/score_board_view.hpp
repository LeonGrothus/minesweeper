#pragma once
#include "api/controller/score_board.hpp"
#include "api/ui/widget/widget.hpp"
#include <memory>

class ScoreBoardView : public Widget {
public:
    explicit ScoreBoardView(const std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& score_board_entries);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D& size) override;

private:
    std::shared_ptr<Widget> m_constructed_widget;
    bool m_is_dirty = true;
};
