#include "bool_setting.hpp"

BoolSetting::BoolSetting(std::u16string description,
                         std::function<void()> on_select,
                         std::function<void()> off_select)
    : ListSetting(std::move(description), true) {
    add_option(ListSettingOption(u"Off", std::move(off_select)));
    add_option(ListSettingOption(u"On", std::move(on_select)));
}
