#pragma once
#include "../widget.hpp"
#include "api/game/board/board_2d.hpp"
#include <memory>

class BoardShowcaseWidget final : public Widget {
public:
    explicit BoardShowcaseWidget(std::shared_ptr<Board2D> board);

    void show_all();

    void hide_all();

    void set_blinking(bool enabled);

    const CanvasElement &build_widget(const Vector2D &size) override;

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

private:
    std::shared_ptr<Board2D> m_board;
    bool m_show_mines = true;
    bool m_blinking_enabled = true;
    double m_blink_timer = 0;
    static constexpr double BLINK_INTERVAL_MS = 500.0; //ms
};
