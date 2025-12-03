#include "board_showcase_widget.hpp"
#include <vector>

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


CanvasElement BoardShowcaseWidget::build_canvas_element(const Vector2D &size) {
    const Vector2D grid_size = m_board->get_grid_size();
    if (grid_size < size) {
        return CanvasElement::empty(size, u' ');
    }

    std::u16string canvas_str;
    canvas_str.reserve(grid_size.area());

    const std::vector<Vector2D> mine_positions = m_board->get_all_mine_positions();

    for (int y = 0; y < grid_size.y; y++) {
        for (int x = 0; x < grid_size.x; x++) {
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
                canvas_str += u'#';
            } else {
                canvas_str += cell.get_representation();
            }
        }
    }

    return CanvasElement(canvas_str, grid_size);
}

Vector2D BoardShowcaseWidget::get_minimum_size() const {
    return m_board->get_grid_size();
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
