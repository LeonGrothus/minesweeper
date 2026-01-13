#include "settings_dialogue.hpp"

#include "../../../../controller/color_manager.hpp"
#include "../selection_widget.hpp"
#include "api/ui/widget/widgets/alignment.hpp"
#include "api/ui/widget/widgets/column.hpp"
#include "api/ui/widget/widgets/custom_drawer.hpp"
#include "api/ui/widget/widgets/padding.hpp"
#include "api/ui/widget/widgets/border/border.hpp"
#include "api/ui/widget/widgets/settings/bool_setting.hpp"
#include "api/ui/widget/widgets/settings/list_setting.hpp"

SettingsDialogue::SettingsDialogue(const std::shared_ptr<SettingsManager>& settings_manager) {
    const std::shared_ptr<CustomDrawer> settings_text = std::make_shared<CustomDrawer>(u"Settings menu!");

    SelectionWidgetOptions selection_options;
    selection_options.blink_highlighted = false;
    selection_options.select_on_enter = false;
    selection_options.parse_keyboard_events_to_hovered = true;
    m_selection_widget = std::make_shared<SelectionWidget>(selection_options);

    const std::shared_ptr<ListSetting> color_option = std::make_shared<ListSetting>(u"Color Option:");
    color_option->add_option(ListSettingOption(u"Monochrome", [settings_manager]() {
        set_terminal_monochrome(1);
        settings_manager->get_settings_mutable().use_color = false;
    }));
    color_option->add_option(ListSettingOption(u"Colorful", [settings_manager]() {
        set_terminal_colored();
        settings_manager->get_settings_mutable().use_color = true;
    }));
    const std::shared_ptr<BoolSetting> show_millis =
        std::make_shared<BoolSetting>(u"Time in ms:",
                                      [settings_manager]() {
                                          settings_manager->get_settings_mutable().show_milliseconds = true;
                                      }, [settings_manager]() {
                                          settings_manager->get_settings_mutable().show_milliseconds = false;
                                      });

    color_option->set_current_index(settings_manager->get_settings().use_color);
    show_millis->set_current_index(settings_manager->get_settings().show_milliseconds);

    m_selection_widget->add_option(color_option);
    m_selection_widget->add_option(show_millis);

    std::vector<std::shared_ptr<Widget>> column_widget{
        std::make_shared<Alignment>(settings_text, MIDDLE_CENTER),
        std::make_shared<Alignment>(m_selection_widget, MIDDLE_CENTER)
    };

    std::shared_ptr<Column> column = std::make_shared<Column>(column_widget);
    column->main_axis_alignment(ListAlignment::Center);

    m_contructed_widget = std::make_shared<Border>(std::make_shared<Padding>(column, 3, 3, 1, 1),
                                                   BorderStyle::double_line_border());
}

void SettingsDialogue::add_custom_option(const std::shared_ptr<ListSetting>& option) const {
    m_selection_widget->add_option(option);
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

CanvasElement SettingsDialogue::build_canvas_element(const Vector2D& size) {
    return m_contructed_widget->build_widget(size);
}

DialogueOptions SettingsDialogue::get_dialogue_options() {
    DialogueOptions options;
    options.update_background = true;
    return options;
}

StackInfo SettingsDialogue::get_stack_info() {
    StackInfo stack_info;
    stack_info.height_percentage = 0.4;
    stack_info.width_percentage = 0.3;
    return stack_info;
}

std::shared_ptr<Dialogue> SettingsDialogue::get_dialogue(const std::shared_ptr<SettingsManager>& settings_manager) {
    return std::make_shared<Dialogue>(std::make_shared<SettingsDialogue>(settings_manager), get_dialogue_options());
}
