#include "board_2d.hpp"

#include <assert.h>
#include <random>

static std::mt19937 &get_rng() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    return rng;
}

Board2D::Board2D(const Vector2D size, const int mines) : m_grid(Grid2D<Cell>(size)), m_mine_count(mines), m_flagged_count(0), m_dirty(true),
                                                         m_is_lost(false), m_is_won(false), m_safe_cells_remaining(size.area() - mines) {
    assert(mines < size.area());
}

void Board2D::first_move(const Vector2D &pos) {
    place_mines(m_mine_count, pos);
    calculate_all_adjacent_mines();
    reveal_next(pos);
    mark_dirty();
}

std::vector<Vector2D> Board2D::reveal_next(const Vector2D &pos) {
    std::vector<Vector2D> revealed_positions;

    if (!m_grid.in_bounds(pos)) {
        return revealed_positions;
    }

    Cell &cell = m_grid.get_at_pos(pos);
    if (cell.is_flagged() || cell.is_revealed()) {
        return revealed_positions;
    }

    cell.reveal();
    revealed_positions.push_back(pos);
    mark_dirty();
    if (cell.contains_mine()) {
        m_is_lost = true;
    } else {
        if (m_safe_cells_remaining > 0) {
            m_safe_cells_remaining--;
            if (m_safe_cells_remaining == 0) {
                m_is_won = true;
            }
        }
    }

    if (cell.contains_mine()) {
        m_reveal_frontier.clear();
        return revealed_positions;
    }

    if (cell.get_adjacent_mines() == 0) {
        m_reveal_frontier.clear();
        for (std::tuple<Vector2D, Cell> &entry: m_grid.get_all_adjacent(pos)) {
            const Vector2D entry_pos = std::get<0>(entry);
            push_if_unrevealed_and_unflagged(entry_pos);
        }
    }

    return revealed_positions;
}

std::vector<Vector2D> Board2D::reveal_step(int max_count) {
    std::vector<Vector2D> revealed;
    int processed = 0;

    while (!m_reveal_frontier.empty() && processed < max_count) {
        Vector2D pos = m_reveal_frontier.front();
        m_reveal_frontier.pop_front();

        Cell &cell = m_grid.get_at_pos(pos);
        if (cell.is_revealed() || cell.is_flagged()) {
            continue;
        }

        if (cell.contains_mine()) {
            continue;
        }

        cell.reveal();
        revealed.push_back(pos);
        processed++;
        mark_dirty();
        if (cell.contains_mine()) {
            m_is_lost = true;
        } else {
            if (m_safe_cells_remaining > 0) {
                m_safe_cells_remaining--;
                if (m_safe_cells_remaining == 0) {
                    m_is_won = true;
                }
            }
        }

        if (cell.get_adjacent_mines() == 0) {
            for (std::tuple<Vector2D, Cell> &entry: m_grid.get_all_adjacent(pos)) {
                if (const Vector2D entry_pos = std::get<0>(entry);
                    !m_grid.get_at_pos(entry_pos).is_revealed() && !m_grid.get_at_pos(entry_pos).is_flagged()) {
                    m_reveal_frontier.push_back(entry_pos);
                }
            }
        }
    }

    return revealed;
}

std::vector<Vector2D> Board2D::reveal_all() {
    std::vector<Vector2D> all;
    while (!m_reveal_frontier.empty()) {
        std::vector<Vector2D> part = reveal_step(256);
        if (part.empty()) {
            break;
        }
        all.insert(all.end(), part.begin(), part.end());
    }
    return all;
}

void Board2D::push_if_unrevealed_and_unflagged(const Vector2D &pos) {
    if (!m_grid.in_bounds(pos)) {
        return;
    }
    if (const Cell &cell = m_grid.get_at_pos(pos); !cell.is_revealed() && !cell.is_flagged()) {
        m_reveal_frontier.push_back(pos);
    }
}

void Board2D::toggle_flag(const Vector2D &pos) {
    Cell &cell = m_grid.get_at_pos(pos);
    const bool was_flagged = cell.is_flagged();
    cell.toggle_flag();

    if (was_flagged) {
        m_flagged_count--;
    } else {
        m_flagged_count++;
    }
    mark_dirty();
}

const Cell &Board2D::get_cell(const Vector2D &pos) {
    return m_grid.get_at_pos(pos);
}

const CanvasElement &Board2D::draw_board() {
    if (!m_dirty) {
        return m_cached_board;
    }

    std::string board;
    board.reserve(m_grid.get_grid_size().area());

    for (int i = 0; i < m_grid.get_grid_size().area(); i++) {
        board += m_grid.get_at_index(i).get_representation();
    }
    m_cached_board = CanvasElement(board, m_grid.get_grid_size());
    m_dirty = false;
    return m_cached_board;
}

void Board2D::place_mines(const int count, const Vector2D &start_pos) {
    const int start_index = m_grid.get_index(start_pos);

    int to_place = count;
    std::vector<int> placed_mines;
    placed_mines.reserve(m_grid.get_grid_size().area());
    for (int i = 0; i < m_grid.get_grid_size().area(); i++) {
        if (i == start_index) {
            continue;
        }
        placed_mines.push_back(i);
    }

    std::mt19937 &rng = get_rng();

    while (to_place-- > 0) {
        std::uniform_int_distribution<int> gen(0, placed_mines.size() - 1);
        const int r = gen(rng);
        const int removed = placed_mines.at(r);
        placed_mines.erase(placed_mines.begin() + r);
        m_grid.get_at_index(removed).set_mine();
    }
}

void Board2D::calculate_all_adjacent_mines() {
    for (int i = 0; i < m_grid.get_grid_size().area(); i++) {
        Vector2D pos = m_grid.get_index_position(i);
        Cell &cell = m_grid.get_at_index(i);
        int mine_count = 0;
        for (std::tuple<Vector2D, Cell> &entry: m_grid.get_all_adjacent(pos)) {
            if (std::get<1>(entry).contains_mine()) {
                mine_count++;
            }
        }
        cell.set_adjacent_mines(mine_count);
    }
}

int Board2D::count_adjacent_mines(const Vector2D &pos) {
    return m_grid.get_at_pos(pos).get_adjacent_mines();
}

void Board2D::mark_dirty() {
    m_dirty = true;
}

bool Board2D::is_dirty() const {
    return m_dirty;
}

bool Board2D::is_won() const {
    return m_is_won;
}

bool Board2D::is_lost() const {
    return m_is_lost;
}

int Board2D::get_flagged_count() const {
    return m_flagged_count;
}
