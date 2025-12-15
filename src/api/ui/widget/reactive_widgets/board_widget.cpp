#include "board_widget.hpp"
#include <algorithm>
#include <curses.h>
#include <string>
#include <vector>

#include "api/helper/conversion_helper.hpp"
#include "api/helper/math_helper.hpp"

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
    if (size < get_minimum_size()) {
        return CanvasElement::empty(size, u' ');
    }

    const auto [board_x, board_y] = m_board->get_grid_size();

    const int max_digits_x = digits(board_x);
    const int max_digits_y = digits(board_y);

    const int effective_x_spacing = std::max(m_x_spacing, max_digits_x - 1) + m_label_spacing;

    //plus 2 for spacing left and right; border is added later
    const int grid_width = board_x + (board_x - 1) * effective_x_spacing + 2;
    const int grid_height = board_y + (board_y - 1) * m_y_spacing;

    std::u16string board_lines;
    board_lines.reserve(grid_width * grid_height);
    for (int y = 0; y < board_y; y++) {
        std::u16string line;
        line.reserve(grid_width);
        line += ' ';
        for (int x = 0; x < board_x; x++) {
            const Vector2D pos{x, y};
            const Cell &cell = m_board->get_cell(pos);

            if (x == m_cursor_pos.x && y == m_cursor_pos.y) {
                line += u'C';
            } else {
                line += cell.get_representation();
            }

            if (x < board_x - 1) {
                line.append(effective_x_spacing, u' ');
            }
        }
        line += ' ';
        board_lines.append(line);
        if (y < board_y - 1) {
            board_lines.append(grid_width * m_y_spacing, u' ');
        }
    }

    //border
    const CanvasElement board_canvas = CanvasElement(board_lines, Vector2D{grid_width, grid_height});
    const CanvasElement bordered_board = CanvasElement::wrap_with_border(board_canvas, m_border_style);

    const int label_width_y = max_digits_y + m_label_spacing;

    const int final_width = label_width_y + bordered_board.get_width();
    const int final_height = 1 + bordered_board.get_height();

    std::u16string final_canvas;
    final_canvas.reserve(final_width * final_height);

    std::u16string x_labels(final_width, u' ');
    for (int x = 0; x < board_x; x++) {
        //left border + left padding + x position in grid
        const int column_offset = 1 + 1 + x * (1 + effective_x_spacing);
        const int target_pos = label_width_y + column_offset;
        const std::u16string number = utf8_to_utf16(std::to_string(x + 1));
        for (int i = 0; i < number.size(); i++) {
            x_labels[target_pos + i] = number[i];
        }
    }
    final_canvas.append(x_labels);

    const std::u16string &bordered_lines = bordered_board.get_canvas_element();
    const int bordered_width = bordered_board.get_width();

    final_canvas.append(label_width_y, u' ');
    final_canvas.append(std::u16string_view(bordered_lines.data(), bordered_width));

    for (int y = 0; y < board_y; y++) {
        //+1 for top border
        const int bordered_row = y * (m_y_spacing + 1) + 1;
        std::u16string_view relevant_line(bordered_lines.data() + bordered_row * bordered_width, bordered_width);
        std::u16string digit(label_width_y, u' ');
        const std::u16string number = utf8_to_utf16(std::to_string(y + 1));
        const int label_start = max_digits_y - static_cast<int>(number.size());

        for (int i = 0; i < number.size(); i++) {
            digit[label_start + i] = number[i];
        }

        final_canvas.append(digit);
        final_canvas.append(relevant_line);

        for (int i = 0; i < m_y_spacing; i++) {
            final_canvas.append(label_width_y, u' ');
            //+1 because the first line was printed already
            const int spacing_row = bordered_row + i + 1;
            final_canvas.append(std::u16string_view(bordered_lines.data() + spacing_row * bordered_width, bordered_width));
        }
    }

    // Bottom border row
    final_canvas.append(label_width_y, u' ');
    final_canvas.append(std::u16string_view(bordered_lines.data() + (bordered_board.get_height() - 1) * bordered_width, bordered_width));

    return CanvasElement(final_canvas, Vector2D{final_width, final_height});
}

Vector2D BoardWidget::get_minimum_size() const {
    const auto [board_x, board_y] = m_board->get_grid_size();

    const int max_digits_x = digits(board_x);
    const int max_digits_y = digits(board_y);

    const int effective_x_spacing = std::max(m_x_spacing, max_digits_x - 1) + m_label_spacing;

    //plus 2 for spacing left and right
    const int grid_width = board_x + (board_x - 1) * effective_x_spacing + 2;
    const int grid_height = board_y + (board_y - 1) * m_y_spacing;

    const int bordered_width = grid_width + 2;
    const int bordered_height = grid_height + 2;

    const int y_label_width = max_digits_y;

    const int final_width = y_label_width + m_label_spacing + bordered_width;
    const int final_height = 1 + bordered_height;

    return Vector2D{final_width, final_height};
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

bool BoardWidget::is_dirty() const {
    return m_is_dirty;
}
