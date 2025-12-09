#include "board_showcase_widget.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "api/helper/conversion_helper.hpp"
#include "api/helper/math_helper.hpp"

BoardShowcaseWidget::BoardShowcaseWidget(std::shared_ptr<Board2D> board)
    : m_board(std::move(board)) {
}

void BoardShowcaseWidget::show_all() {
    m_show_mines = true;
    m_is_dirty = true;
}

void BoardShowcaseWidget::hide_all() {
    m_show_mines = false;
    m_is_dirty = true;
}

void BoardShowcaseWidget::set_blinking(const bool enabled) {
    m_blinking_enabled = enabled;
    m_blink_timer = 0.0;
    m_is_dirty = true;
}

void BoardShowcaseWidget::set_x_spacing(const int spacing) {
    m_x_spacing = spacing;
    m_is_dirty = true;
}

void BoardShowcaseWidget::set_y_spacing(const int spacing) {
    m_y_spacing = spacing;
    m_is_dirty = true;
}

void BoardShowcaseWidget::set_border_style(const BorderStyle &style) {
    m_border_style = style;
    m_is_dirty = true;
}

CanvasElement BoardShowcaseWidget::build_canvas_element(const Vector2D &size) {
    if (size < get_minimum_size()) {
        return CanvasElement::empty(size, u' ');
    }

    const auto [board_x, board_y] = m_board->get_grid_size();
    const std::vector<Vector2D> mine_positions = m_board->get_all_mine_positions();

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

            bool is_mine = false;
            for (const auto &[mine_x, mine_y]: mine_positions) {
                if (mine_x == x && mine_y == y) {
                    is_mine = true;
                    break;
                }
            }

            if (is_mine && m_show_mines) {
                line += u'X';
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

    //digit placement
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
    final_canvas.append(label_width_y, u' ');
    final_canvas.append(std::u16string_view(bordered_lines.data() + (bordered_board.get_height() - 1) * bordered_width, bordered_width));

    return CanvasElement(final_canvas, Vector2D{final_width, final_height});
}

Vector2D BoardShowcaseWidget::get_minimum_size() const {
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

void BoardShowcaseWidget::keyboard_press(int key) {
}

void BoardShowcaseWidget::update(const double delta_time) {
    if (!m_blinking_enabled) {
        return;
    }

    m_blink_timer += delta_time;
    if (m_blink_timer >= BLINK_INTERVAL_MS) {
        m_blink_timer -= BLINK_INTERVAL_MS;
        m_show_mines = !m_show_mines;
        m_is_dirty = true;
    }
}

bool BoardShowcaseWidget::is_dirty() const {
    return m_is_dirty;
}
