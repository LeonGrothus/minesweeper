#pragma once
#include "../../widget.hpp"
#include "api/game/board/board_2d.hpp"
#include "api/ui/widget/widgets/border/border_style.hpp"
#include <memory>

class BoardShowcaseWidget final : public Widget {
public:
    explicit BoardShowcaseWidget(std::shared_ptr<Board2D> board);

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
    std::shared_ptr<Board2D> m_board;
    bool m_show_mines = true;
    bool m_blinking_enabled = true;
    double m_blink_timer = 0;

    int m_x_spacing = 1;
    int m_y_spacing = 0;
    int m_label_spacing = 1;

    BorderStyle m_border_style = BorderStyle::double_line_border();
    static constexpr double BLINK_INTERVAL_MS = 500.0; //ms
};
