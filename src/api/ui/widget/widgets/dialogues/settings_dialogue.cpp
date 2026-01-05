#include "settings_dialogue.hpp"

#include "api/ui/widget/reactive_widgets/selection_widget.hpp"
#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/column.hpp"
#include "api/ui/widget/widgets/custom_drawer.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/border/border.hpp"
#include "api/ui/widget/widgets/settings/bool_setting.hpp"
#include "api/ui/widget/widgets/settings/list_setting.hpp"

SettingsDialogue::SettingsDialogue() {
    const std::shared_ptr<CustomDrawer> settings_text = std::make_shared<CustomDrawer>(u"Settings menu!");

    SelectionWidgetOptions selection_options;
    selection_options.blink_highlighted = false;
    selection_options.select_on_enter = false;
    selection_options.parse_keyboard_events_to_hovered = true;
    const std::shared_ptr<SelectionWidget> list = std::make_shared<SelectionWidget>(selection_options);

    const std::shared_ptr<ListSetting> color_option = std::make_shared<ListSetting>(u"Color Option:");
    color_option->add_option(ListSettingOption(u"Colorful"));
    color_option->add_option(ListSettingOption(u"Monochrome"));
    const std::shared_ptr<BoolSetting> show_millis = std::make_shared<BoolSetting>(u"Time in ms:");

    list->add_option(color_option);
    list->add_option(show_millis);

    std::vector<std::shared_ptr<Widget> > column_widget{
        std::make_shared<Alignment>(settings_text, MIDDLE_CENTER),
        std::make_shared<Alignment>(list, MIDDLE_CENTER)
    };

    std::shared_ptr<Column> column = std::make_shared<Column>(column_widget);
    column->main_axis_alignment(ListAlignment::Center);

    m_contructed_widget = std::make_shared<Border>(std::make_shared<Padding>(column, 3, 3, 1, 1), BorderStyle::double_line_border());
}

Vector2D SettingsDialogue::get_minimum_size() const {
    return m_contructed_widget->get_minimum_size();
}

void SettingsDialogue::keyboard_press(const int key) {
    m_contructed_widget->keyboard_press(key);
}

void SettingsDialogue::update(const double delta_time) {
    m_contructed_widget->update(delta_time);
}

bool SettingsDialogue::is_dirty() const {
    return m_is_dirty || m_contructed_widget->is_dirty();
}

CanvasElement SettingsDialogue::build_canvas_element(const Vector2D &size) {
    return m_contructed_widget->build_widget(size);
}
