#pragma once
#include <cstdint>

short get_color_for_role(uint8_t role);

void init_terminal_colors();

void set_terminal_colored();

void set_terminal_monochrome(short role);
