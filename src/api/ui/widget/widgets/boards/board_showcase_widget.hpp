#pragma once
#include "../../widget.hpp"
#include "api/game/board/board_2d.hpp"
#include "api/ui/widget/widgets/border/border_style.hpp"
#include <memory>

#include "board_2d_widget.hpp"

class BoardShowcaseWidget final : public Widget {
public:
    explicit BoardShowcaseWidget(const std::shared_ptr<Board2D> &board);

    void set_blinking(bool enabled);

    void set_x_spacing(int spacing);

    void set_y_spacing(int spacing);

    void set_border_style(const BorderStyle &style);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    void rebuild_layout();

    std::shared_ptr<Board2dWidget> m_board_widget;
    std::shared_ptr<Widget> m_layout_widget;

    int m_x_spacing = 2;
    int m_y_spacing = 0;

    BorderStyle m_border_style = BorderStyle::double_line_border();
};
