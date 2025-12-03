#pragma once
#include <string>

std::string utf16_to_utf8(const std::u16string &input);

std::u16string utf8_to_utf16(const std::string &input);
