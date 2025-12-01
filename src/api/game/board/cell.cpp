#include "cell.hpp"

Cell::Cell() : m_has_mine(false), m_is_revealed(false), m_is_flagged(false), m_adjacent_mines(0) {
}

void Cell::reveal() {
    if (!m_is_flagged) {
        m_is_revealed = true;
    }
}

void Cell::toggle_flag() {
    if (!m_is_revealed) {
        m_is_flagged = !m_is_flagged;
    }
}

void Cell::set_mine() {
    m_has_mine = true;
}

void Cell::set_adjacent_mines(const int mines) {
    m_adjacent_mines = mines;
}

int Cell::get_adjacent_mines() const {
    return m_adjacent_mines;
}

bool Cell::contains_mine() const {
    return m_has_mine;
}

bool Cell::is_revealed() const {
    return m_is_revealed;
}

bool Cell::is_flagged() const {
    return m_is_flagged;
}

char Cell::get_representation() const {
    if (!m_is_revealed) {
        if (m_is_flagged) {
            return 'F';
        }
        return '#';
    }

    if (m_has_mine) {
        return 'X';
    }

    if (m_adjacent_mines == 0) {
        return ' ';
    }
    return '0' + m_adjacent_mines;
}
