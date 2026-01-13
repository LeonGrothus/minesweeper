#include "score_board_view.hpp"

#include "alignment.hpp"
#include "column.hpp"
#include "custom_drawer.hpp"
#include "padding.hpp"
#include "row.hpp"
#include "timer.hpp"
#include "rainbow_switcher.hpp"
#include "border/border.hpp"
#include <memory>

#include "api/controller/color_manager.hpp"
#include "api/controller/terminal_controller.hpp"

ScoreBoardView::ScoreBoardView(const std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& score_board_entries) {
    std::vector<std::shared_ptr<Widget>> score_rows;

    for (int i = 0; i < SCORE_BOARD_NUMBER; i++) {
        const auto& [score_name, score_time] = score_board_entries[i];

        ColorRole rank_color;
        switch (i) {
        case 1:
            rank_color = ColorRole::Number2;
            break;
        case 2:
            rank_color = ColorRole::Number3;
            break;
        default:
            rank_color = ColorRole::Text;
            break;
        }

        const std::u16string rank_number = std::u16string(1, static_cast<char16_t>(u'1' + i));
        const std::shared_ptr<Widget> rank_widget = std::make_shared<Alignment>(
            std::make_shared<CustomDrawer>(rank_number), MIDDLE_LEFT);

        std::u16string name_text = score_name.empty()
                                       ? u"-"
                                       : std::u16string(score_name.begin(), score_name.end());

        name_text.append(MAX_NAME_LENGTH - static_cast<int>(name_text.length()), EMPTY_CHAR);

        const std::shared_ptr<Widget> name_widget = std::make_shared<CustomDrawer>(name_text);

        std::shared_ptr<Timer> time_widget = std::make_shared<Timer>(false, true);;
        if (score_time > 0) {
            time_widget->set_time_from_millis(score_time);
        }
        time_widget->stop();

        const std::shared_ptr<Widget> time_aligned = std::make_shared<Alignment>(time_widget, MIDDLE_RIGHT);

        std::vector<std::shared_ptr<Widget>> row_items{
            rank_widget,
            std::make_shared<CustomDrawer>(u": "),
            name_widget,
            time_aligned
        };

        std::shared_ptr<Row> row = std::make_shared<Row>(row_items);
        row->main_axis_alignment(ListAlignment::None);

        std::shared_ptr<Widget> final_row;
        if (i == 0) {
            final_row = std::make_shared<RainbowSwitcher>(row, get_all_colors_except_black(), true);
        } else {
            final_row = std::make_shared<RainbowSwitcher>(row, rank_color);
        }

        score_rows.push_back(final_row);
    }

    std::shared_ptr<Column> column = std::make_shared<Column>(score_rows);
    column->main_axis_alignment(ListAlignment::Center);

    m_constructed_widget = std::make_shared<Border>(
        std::make_shared<Padding>(column, 2, 2, 1, 1),
        BorderStyle::single_line_border()
    );
}

Vector2D ScoreBoardView::get_minimum_size() const {
    return m_constructed_widget->get_minimum_size();
}

void ScoreBoardView::keyboard_press(const int key) {
    m_constructed_widget->keyboard_press(key);
}

void ScoreBoardView::update(const double delta_time) {
    m_constructed_widget->update(delta_time);
}

bool ScoreBoardView::is_dirty() const {
    return m_is_dirty || m_constructed_widget->is_dirty();
}

CanvasElement ScoreBoardView::build_canvas_element(const Vector2D& size) {
    m_is_dirty = false;
    return m_constructed_widget->build_widget(size);
}
