#pragma once

class Cell {
public:
    explicit Cell();

    void reveal();

    void toggle_flag();

    void set_mine();

    void set_adjacent_mines(int mines);

    int get_adjacent_mines() const;

    bool has_mine() const;

    bool is_revealed() const;

    bool is_flagged() const;

    char16_t get_representation() const;

private:
    bool m_has_mine;
    bool m_is_revealed;
    bool m_is_flagged;

    int m_adjacent_mines;
};
