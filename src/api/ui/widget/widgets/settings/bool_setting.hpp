#pragma once
#include "list_setting.hpp"

class BoolSetting : public ListSetting {
public:
    explicit BoolSetting(std::u16string description, 
                        std::function<void()> on_select = []() {},
                        std::function<void()> off_select = []() {});
};
