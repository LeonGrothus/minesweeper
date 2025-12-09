#pragma once

struct BorderStyle {
    char16_t left;
    char16_t top;
    char16_t right;
    char16_t bottom;
    char16_t top_left_corner;
    char16_t top_right_corner;
    char16_t bottom_right_corner;
    char16_t bottom_left_corner;

    static BorderStyle double_line_border();

    static BorderStyle single_line_border();

    static BorderStyle single_thick_border();

    static BorderStyle dotted_border();

    static BorderStyle dotted_thick_border();

    static BorderStyle block_border();

    static BorderStyle of_char(char16_t character);

private:
    explicit BorderStyle(char16_t left, char16_t top, char16_t right, char16_t bottom,
                         char16_t top_left_corner, char16_t top_right_corner,
                         char16_t bottom_right_corner, char16_t bottom_left_corner);
};
