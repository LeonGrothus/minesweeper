#include "inform_dialogue.hpp"

#include <utility>

#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/column.hpp"
#include "api/ui/widget/widgets/custom_drawer.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/border/border.hpp"

InformDialogue::InformDialogue(std::shared_ptr<Widget> main_widget, const std::u16string &left_text, const std::u16string &right_text,
                               const std::function<void()> &left_callback,
                               const std::function<void()> &right_callback) {
    SelectionWidgetOptions options;
    options.is_vertical = false;
    options.spacing_options = 3;
    options.make_all_same_size = false;
    m_selection = std::make_shared<SelectionWidget>(options);
    m_selection->add_option(std::make_shared<CustomDrawer>(right_text), right_callback);
    m_selection->add_option(std::make_shared<CustomDrawer>(left_text), left_callback);

    std::vector<std::shared_ptr<Widget> > column_list{
        std::make_shared<Alignment>(std::move(main_widget), BOTTOM_CENTER), std::make_shared<Alignment>(m_selection, BOTTOM_CENTER)
    };
    std::shared_ptr<Column> column = std::make_shared<Column>(column_list);
    column->main_axis_alignment(ListAlignment::Center);
    m_contructed_widget = std::make_shared<Border>(std::make_shared<Padding>(column, 3, 3, 1, 1), BorderStyle::double_line_border());
}

Vector2D InformDialogue::get_minimum_size() const {
    return m_contructed_widget->get_minimum_size();
}

void InformDialogue::keyboard_press(const int key) {
    m_selection->keyboard_press(key);
}

void InformDialogue::update(const double delta_time) {
    m_contructed_widget->update(delta_time);
}

bool InformDialogue::is_dirty() const {
    return m_is_dirty || m_contructed_widget->is_dirty();
}

CanvasElement InformDialogue::build_canvas_element(const Vector2D &size) {
    return m_contructed_widget->build_widget(size);
}
