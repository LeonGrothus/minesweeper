#pragma once
#include <memory>

#include "api/ui/widget/widget.hpp"

class Board2D;

class Board2dWidget : public Widget {
public:
    explicit Board2dWidget(const std::shared_ptr<Board2D> &board);

    void show_all_mines();

    void hide_all_mines();

    void set_x_spacing(int spacing);

    void set_y_spacing(int spacing);

    void set_interactable(bool interactable);

    void set_blinking(bool blinking);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    std::shared_ptr<Board2D> m_board;

    Vector2D m_cursor_pos{0, 0};
    bool m_first_move_done = false;
    double m_reveal_timer = 0;
    bool m_is_revealing = false;

    int m_x_spacing = 1;
    int m_y_spacing = 0;

    bool m_blink_mines = false;
    double m_blink_timer = 0;

    bool m_show_mines = false;

    bool m_interactable = false;

    static constexpr double REVEAL_INTERVAL = 20.0; //ms
    static constexpr double BLINK_INTERVAL = 500.0; //ms

    void handle_reveal();

    void handle_flag();
};
