#include "board_2d_widget.hpp"

#include <curses.h>
#include <vector>

#include "api/game/board/board_2d.hpp"
#include "api/ui/canvas/canvas_element.hpp"

ColorRole get_role_for_cell(const Cell &cell, const bool show_mines, const bool is_mine_position) {
    if (!cell.is_revealed()) {
        if (cell.is_flagged()) {
            return ColorRole::Flag;
        }
        if (show_mines && is_mine_position) {
            return ColorRole::Mine;
        }
        return ColorRole::Hidden;
    }

    if (cell.has_mine()) {
        return ColorRole::Mine;
    }

    switch (cell.get_adjacent_mines()) {
        case 1: return ColorRole::Number1;
        case 2: return ColorRole::Number2;
        case 3: return ColorRole::Number3;
        case 4: return ColorRole::Number4;
        case 5: return ColorRole::Number5;
        case 6: return ColorRole::Number6;
        case 7: return ColorRole::Number7;
        case 8: return ColorRole::Number8;
        default: return ColorRole::Text;
    }
}

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
    std::vector<uint8_t> color_roles;
    board_lines.reserve(grid_width * grid_height);
    color_roles.reserve(grid_width * grid_height);
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

            ColorRole role = ColorRole::Hidden;
            if (m_interactable && (x == m_cursor_pos.x && y == m_cursor_pos.y)) {
                line += u'C';
                role = ColorRole::Cursor;
            } else {
                const char16_t glyph = cell.get_representation(is_mine && m_show_mines);
                line += glyph;
                role = get_role_for_cell(cell, m_show_mines, is_mine);
            }
            color_roles.push_back(static_cast<uint8_t>(role));

            if (x < board_x - 1) {
                line.append(m_x_spacing, u' ');
                color_roles.insert(color_roles.end(), m_x_spacing, static_cast<uint8_t>(ColorRole::Default));
            }
        }
        board_lines.append(line);
        if (y < board_y - 1) {
            board_lines.append(grid_width * m_y_spacing, u' ');
            color_roles.insert(color_roles.end(), grid_width * m_y_spacing, static_cast<uint8_t>(ColorRole::Default));
        }
    }
    return CanvasElement(board_lines, color_roles, Vector2D{grid_width, grid_height});
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
