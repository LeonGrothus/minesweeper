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

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Board2D> m_board;
    bool m_show_mines = true;
    bool m_blinking_enabled = true;
    double m_blink_timer = 0;
    static constexpr double BLINK_INTERVAL_MS = 500.0; //ms
};
