#include "board_showcase_widget.hpp"

#include "api/helper/math_helper.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/border/border.hpp"
#include "api/ui/widget/widgets/enumeration_widget.hpp"

BoardShowcaseWidget::BoardShowcaseWidget(const std::shared_ptr<Board2D> &board)
    : m_board_2d(board), m_board_widget(std::make_shared<Board2DWidget>(board)) {
    rebuild_layout();
}

void BoardShowcaseWidget::rebuild_layout() {
    m_board_widget->show_all_mines();

    m_board_widget->set_x_spacing(m_x_spacing + digits(m_board_2d->get_grid_size().x) - 1);
    m_board_widget->set_y_spacing(m_y_spacing);

    std::shared_ptr<Widget> padded = std::make_shared<Padding>(m_board_widget, 1, 1, 0, 0);
    std::shared_ptr<Border> inner_border = std::make_shared<Border>(padded, m_border_style);
    m_layout_widget = std::make_shared<EnumerationWidget>(inner_border, 2, 1, 1, 1, m_x_spacing, m_y_spacing);

    m_is_dirty = true;
}

void BoardShowcaseWidget::set_blinking(const bool enabled) {
    m_board_widget->set_blinking(enabled);
    m_is_dirty = true;
}

void BoardShowcaseWidget::set_x_spacing(const int spacing) {
    m_x_spacing = spacing;
    rebuild_layout();
}

void BoardShowcaseWidget::set_y_spacing(const int spacing) {
    m_y_spacing = spacing;
    rebuild_layout();
}

void BoardShowcaseWidget::set_border_style(const BorderStyle &style) {
    m_border_style = style;
    rebuild_layout();
}

CanvasElement BoardShowcaseWidget::build_canvas_element(const Vector2D &size) {
    return m_layout_widget->build_widget(size);
}

Vector2D BoardShowcaseWidget::get_minimum_size() const {
    return m_layout_widget->get_minimum_size();
}

void BoardShowcaseWidget::keyboard_press(const int key) {
    m_layout_widget->keyboard_press(key);
}

void BoardShowcaseWidget::update(const double delta_time) {
    m_layout_widget->update(delta_time);
}

bool BoardShowcaseWidget::is_dirty() const {
    return m_is_dirty || m_layout_widget->is_dirty();
}
