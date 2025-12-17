#include "board_2d_widget.hpp"

#include <curses.h>

#include "api/game/board/board_2d.hpp"

Board2dWidget::Board2dWidget(const std::shared_ptr<Board2D> &board) : m_board(board) {
}

void Board2dWidget::show_all_mines() {
    m_show_mines = true;
}

void Board2dWidget::hide_all_mines() {
    m_show_mines = false;
}

void Board2dWidget::set_x_spacing(const int spacing) {
    m_x_spacing = spacing;
    m_is_dirty = true;
}

void Board2dWidget::set_y_spacing(const int spacing) {
    m_y_spacing = spacing;
    m_is_dirty = true;
}

void Board2dWidget::set_interactable(const bool interactable) {
    m_interactable = interactable;
}

void Board2dWidget::set_blinking(const bool blinking) {
    m_blink_mines = blinking;
}

void Board2dWidget::handle_reveal() {
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

void Board2dWidget::handle_flag() {
    if (m_board->is_won() || m_board->is_lost()) {
        return;
    }

    if (const Cell &cell = m_board->get_cell(m_cursor_pos); !cell.is_revealed()) {
        m_board->toggle_flag(m_cursor_pos);
        m_is_dirty = true;
    }
}

Vector2D Board2dWidget::get_minimum_size() const {
    const auto [board_x, board_y] = m_board->get_grid_size();

    const int grid_width = board_x + (board_x - 1) * m_x_spacing;
    const int grid_height = board_y + (board_y - 1) * m_y_spacing;

    return Vector2D{grid_width, grid_height};
}

void Board2dWidget::update(const double delta_time) {
    if (m_blink_mines) {
        m_blink_timer += delta_time;
        if (m_blink_timer >= BLINK_INTERVAL) {
            m_blink_timer -= BLINK_INTERVAL;
            m_show_mines = !m_show_mines;
            m_is_dirty = true;
        }
    }

    if (!m_is_revealing || !m_interactable) {
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

bool Board2dWidget::is_dirty() const {
    return m_is_dirty;
}

CanvasElement Board2dWidget::build_canvas_element(const Vector2D &size) {
    const auto [board_x, board_y] = m_board->get_grid_size();
    const std::vector<Vector2D> mine_positions = m_board->get_all_mine_positions();

    auto [grid_width, grid_height] = get_minimum_size();

    std::u16string board_lines;
    board_lines.reserve(grid_width * grid_height);
    for (int y = 0; y < board_y; y++) {
        std::u16string line;
        line.reserve(grid_width);
        for (int x = 0; x < board_x; x++) {
            const Vector2D pos{x, y};
            const Cell &cell = m_board->get_cell(pos);

            bool is_mine = false;
            for (const auto &[mine_x, mine_y]: mine_positions) {
                if (mine_x == x && mine_y == y) {
                    is_mine = true;
                    break;
                }
            }

            if (m_interactable && (x == m_cursor_pos.x && y == m_cursor_pos.y)) {
                line += u'C';
            } else {
                line += cell.get_representation(is_mine && m_show_mines);
            }

            if (x < board_x - 1) {
                line.append(m_x_spacing, u' ');
            }
        }
        board_lines.append(line);
        if (y < board_y - 1) {
            board_lines.append(grid_width * m_y_spacing, u' ');
        }
    }
    return CanvasElement(board_lines, size);
}

void Board2dWidget::keyboard_press(const int key) {
    if (!m_interactable) {
        return;
    }

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
        default:
            break;
    }
}
