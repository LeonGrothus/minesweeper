#include "board_2d.hpp"

#include <algorithm>
#include <cassert>
#include <unordered_set>

#include "api/helper/random_helper.hpp"

Board2D::Board2D(const Vector2D size, const int mines, const bool force_mines) : m_grid(Grid2D<Cell>(size)),
                                                                                 m_mine_count(mines),
                                                                                 m_flagged_count(0),
                                                                                 m_is_lost(false), m_is_won(false),
                                                                                 m_safe_cells_remaining(size.area() - mines) {
    assert(mines < size.area());
    if (force_mines) {
        place_mines(mines, -1);
    }
}

Board2D::Board2D(const Vector2D size, const float mines_percentage, const bool force_mines) : m_grid(Grid2D<Cell>(size)),
    m_flagged_count(0), m_is_lost(false), m_is_won(false) {
    assert(mines_percentage <= 1);
    const int mines = static_cast<int>(mines_percentage * static_cast<float>(size.area()));
    m_mine_count = mines;
    m_safe_cells_remaining = size.area() - mines;
    if (force_mines) {
        place_mines(mines, -1);
    }
}

void Board2D::first_move(const Vector2D& pos) {
    place_mines(m_mine_count, pos);
    calculate_all_adjacent_mines();
    reveal_next(pos);
}

std::vector<Vector2D> Board2D::reveal_next(const Vector2D& pos) {
    std::vector<Vector2D> revealed_positions;

    if (!m_grid.in_bounds(pos)) {
        return revealed_positions;
    }

    Cell& cell = m_grid.get_at_pos(pos);
    if (cell.is_flagged() || cell.is_revealed()) {
        return revealed_positions;
    }
    m_moves_count++;

    cell.reveal();
    revealed_positions.push_back(pos);

    if (cell.has_mine()) {
        m_is_lost = true;
    } else {
        if (m_safe_cells_remaining > 0) {
            m_safe_cells_remaining--;
            check_win();
        }
    }

    if (cell.has_mine()) {
        m_reveal_frontier.clear();
        return revealed_positions;
    }

    if (cell.get_adjacent_mines() == 0) {
        m_reveal_frontier.clear();
        for (std::tuple<Vector2D, Cell>& entry : m_grid.get_all_adjacent(pos)) {
            const Vector2D entry_pos = std::get<0>(entry);
            push_if_unrevealed_and_unflagged(entry_pos);
        }
    }

    return revealed_positions;
}

std::vector<Vector2D> Board2D::reveal_step(const int max_count) {
    std::vector<Vector2D> revealed;
    int processed = 0;

    while (!m_reveal_frontier.empty() && processed < max_count) {
        Vector2D pos = m_reveal_frontier.front();
        m_reveal_frontier.pop_front();

        Cell& cell = m_grid.get_at_pos(pos);
        if (cell.is_revealed() || cell.is_flagged()) {
            continue;
        }

        if (cell.has_mine()) {
            continue;
        }

        cell.reveal();
        revealed.push_back(pos);
        processed++;

        if (cell.has_mine()) {
            m_is_lost = true;
        } else {
            if (m_safe_cells_remaining > 0) {
                m_safe_cells_remaining--;
                check_win();
            }
        }

        if (cell.get_adjacent_mines() == 0) {
            for (std::tuple<Vector2D, Cell>& entry : m_grid.get_all_adjacent(pos)) {
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

void Board2D::push_if_unrevealed_and_unflagged(const Vector2D& pos) {
    if (!m_grid.in_bounds(pos)) {
        return;
    }
    if (const Cell& cell = m_grid.get_at_pos(pos); !cell.is_revealed() && !cell.is_flagged()) {
        m_reveal_frontier.push_back(pos);
    }
}

void Board2D::toggle_flag(const Vector2D& pos) {
    Cell& cell = m_grid.get_at_pos(pos);
    const bool was_flagged = cell.is_flagged();
    cell.toggle_flag();

    if (was_flagged) {
        m_flagged_count--;
    } else {
        m_flagged_count++;
        check_win();
    }
}

const Cell& Board2D::get_cell(const Vector2D& pos) {
    return m_grid.get_at_pos(pos);
}

void Board2D::place_mines(const int count, const Vector2D& start_pos) {
    const int start_index = m_grid.get_index(start_pos);
    place_mines(count, start_index);
}

void Board2D::place_mines(const int count, const int start_index) {
    std::vector<int> placeable_mines;

    //O(1) lookup, faster than vector
    std::unordered_set<int> excluded_indices;
    excluded_indices.insert(start_index);

    for (const std::tuple<Vector2D, Cell>& item : m_grid.get_all_adjacent(m_grid.get_index_position(start_index))) {
        excluded_indices.insert(m_grid.get_index(std::get<0>(item)));
    }

    placeable_mines.reserve(m_grid.get_grid_size().area() - excluded_indices.size());
    for (int i = 0; i < m_grid.get_grid_size().area(); i++) {
        if (!excluded_indices.contains(i)) {
            placeable_mines.push_back(i);
        }
    }

    std::mt19937& rng = get_rng();
    std::ranges::shuffle(placeable_mines.begin(), placeable_mines.end(), rng);

    for (int i = 0; i < count && !placeable_mines.empty(); i++) {
        const int removed = placeable_mines.back();
        placeable_mines.pop_back();
        m_grid.get_at_index(removed).set_mine();
    }
}

void Board2D::check_win() {
    if (m_safe_cells_remaining == 0 && m_flagged_count == m_mine_count) {
        m_is_won = true;
    }
}

void Board2D::calculate_all_adjacent_mines() {
    for (int i = 0; i < m_grid.get_grid_size().area(); i++) {
        Vector2D pos = m_grid.get_index_position(i);
        Cell& cell = m_grid.get_at_index(i);
        int mine_count = 0;
        for (std::tuple<Vector2D, Cell>& entry : m_grid.get_all_adjacent(pos)) {
            if (std::get<1>(entry).has_mine()) {
                mine_count++;
            }
        }
        cell.set_adjacent_mines(mine_count);
    }
}

int Board2D::count_adjacent_mines(const Vector2D& pos) {
    return m_grid.get_at_pos(pos).get_adjacent_mines();
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

int Board2D::get_mine_count() const {
    return m_mine_count;
}

int Board2D::get_total_moves_count() const {
    return m_moves_count;
}

Vector2D Board2D::get_grid_size() const {
    return m_grid.get_grid_size();
}

std::vector<Vector2D> Board2D::get_all_mine_positions() const {
    std::vector<Vector2D> mine_positions;
    mine_positions.reserve(m_mine_count);
    for (int i = 0; i < m_grid.get_grid_size().area(); i++) {
        if (const Cell& cell = m_grid.get_at_index(i); cell.has_mine()) {
            mine_positions.push_back(m_grid.get_index_position(i));
        }
    }
    return mine_positions;
}
