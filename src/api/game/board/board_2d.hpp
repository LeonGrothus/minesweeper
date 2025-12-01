#pragma once
#include <deque>

#include "cell.hpp"
#include "grid_2d.hpp"

class Board2D {
public:
    explicit Board2D(Vector2D size, int mines);

    void first_move(const Vector2D &pos);

    std::vector<Vector2D> reveal_next(const Vector2D &pos);

    std::vector<Vector2D> reveal_step(int max_count = 1);

    std::vector<Vector2D> reveal_all();

    void toggle_flag(const Vector2D &pos);

    const Cell &get_cell(const Vector2D &pos);

    const CanvasElement &draw_board();

    bool is_dirty() const;

    bool is_won() const;

    bool is_lost() const;

    int get_flagged_count() const;

private:
    Grid2D<Cell> m_grid;
    int m_mine_count;
    int m_flagged_count;
    bool m_dirty;
    
    bool m_is_lost;
    bool m_is_won;
    int m_safe_cells_remaining;

    std::deque<Vector2D> m_reveal_frontier;

    CanvasElement m_cached_board{CanvasElement("")};

    void place_mines(int count, const Vector2D &start_pos);

    void mark_dirty();

    void calculate_all_adjacent_mines();

    int count_adjacent_mines(const Vector2D &pos);

    void push_if_unrevealed_and_unflagged(const Vector2D &pos);
};
