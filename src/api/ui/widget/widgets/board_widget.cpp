#include "board_widget.hpp"
#include <curses.h>
#include <vector>

BoardWidget::BoardWidget(std::shared_ptr<Board2D> board)
    : m_board(std::move(board)), m_cursor_pos{0, 0}, m_first_move_done(false),
      m_reveal_timer(0.0), m_is_revealing(false), m_mine_count(m_board->get_mine_count()) {
}

void BoardWidget::reset_game() {
    m_first_move_done = false;
    m_cursor_pos = {0, 0};
    m_is_revealing = false;
    m_reveal_timer = 0.0;

    const Vector2D size = m_board->get_grid_size();
    m_board = std::make_shared<Board2D>(size, m_mine_count);
    m_is_dirty = true;
}

void BoardWidget::handle_reveal() {
    if (m_board->is_won() || m_board->is_lost()) {
        return;
    }

    if (const Cell &cell = m_board->get_cell(m_cursor_pos); cell.is_revealed() || cell.is_flagged()) {
        return;
    }

    if (!m_first_move_done) {
        m_board->first_move(m_cursor_pos);
        m_first_move_done = true;
        m_is_revealing = true;
        m_reveal_timer = 0.0;
    } else {
        m_board->reveal_next(m_cursor_pos);
        m_is_revealing = true;
        m_reveal_timer = 0.0;
    }

    m_is_dirty = true;
}

void BoardWidget::handle_flag() {
    if (m_board->is_won() || m_board->is_lost()) {
        return;
    }

    if (const Cell &cell = m_board->get_cell(m_cursor_pos); !cell.is_revealed()) {
        m_board->toggle_flag(m_cursor_pos);
        m_is_dirty = true;
    }
}

CanvasElement BoardWidget::build_canvas_element(const Vector2D &size) {
    const Vector2D grid_size = m_board->get_grid_size();
    std::u16string canvas_str;
    canvas_str.reserve(grid_size.area());

    for (int y = 0; y < grid_size.y; y++) {
        for (int x = 0; x < grid_size.x; x++) {
            const Vector2D pos{x, y};
            const Cell &cell = m_board->get_cell(pos);

            if (x == m_cursor_pos.x && y == m_cursor_pos.y) {
                canvas_str += u'C';
                // if (!cell.is_revealed()) {
                //     canvas_str += 'C';
                // } else {
                //     canvas_str += cell.get_representation();
                // }
            } else {
                canvas_str += cell.get_representation();
            }
        }
    }
    return CanvasElement(canvas_str, grid_size);;
}

Vector2D BoardWidget::get_minimum_size() const {
    return m_board->get_grid_size();
}

void BoardWidget::keyboard_press(const int key) {
    const auto [board_size_x, board_size_y] = m_board->get_grid_size();

    switch (key) {
        case KEY_UP:
        case 'w':
        case 'W':
            if (m_cursor_pos.y > 0) {
                m_cursor_pos.y--;
                m_is_dirty = true;
            }
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            if (m_cursor_pos.y < board_size_y - 1) {
                m_cursor_pos.y++;
                m_is_dirty = true;
            }
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
            if (m_cursor_pos.x > 0) {
                m_cursor_pos.x--;
                m_is_dirty = true;
            }
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            if (m_cursor_pos.x < board_size_x - 1) {
                m_cursor_pos.x++;
                m_is_dirty = true;
            }
            break;
        case ' ':
        case '\n':
        case KEY_ENTER:
            handle_reveal();
            break;
        case 'f':
        case 'F':
            handle_flag();
            break;
        case 'r':
        case 'R':
            reset_game();
            break;
        default:
            break;
    }
}

void BoardWidget::update(const double delta_time) {
    if (!m_is_revealing) {
        return;
    }

    m_reveal_timer += delta_time;
    if (m_reveal_timer >= REVEAL_INTERVAL) {
        m_reveal_timer -= REVEAL_INTERVAL;

        if (const std::vector<Vector2D> revealed = m_board->reveal_step(1); revealed.empty()) {
            m_is_revealing = false;
        } else {
            m_is_dirty = true;
        }
    }
}
