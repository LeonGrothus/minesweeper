#pragma once
#include "../widget.hpp"
#include "api/game/board/board_2d.hpp"
#include <memory>

class BoardWidget final : public Widget {
public:
    explicit BoardWidget(std::shared_ptr<Board2D> board);

    void reset_game();

    const CanvasElement &build_widget(const Vector2D &size) override;

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

private:
    std::shared_ptr<Board2D> m_board;
    Vector2D m_cursor_pos;
    bool m_first_move_done;
    double m_reveal_timer;
    bool m_is_revealing;
    int m_mine_count;
    static constexpr double REVEAL_INTERVAL = 50.0; //ms

    void handle_reveal();

    void handle_flag();
};
