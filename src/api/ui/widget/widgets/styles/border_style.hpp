#pragma once

struct PaddingBorderStyle {
    char16_t left;
    char16_t top;
    char16_t right;
    char16_t bottom;
    char16_t top_left_corner;
    char16_t top_right_corner;
    char16_t bottom_right_corner;
    char16_t bottom_left_corner;

    static PaddingBorderStyle double_line_border();

    static PaddingBorderStyle single_line_border();

    static PaddingBorderStyle single_thick_border();

    static PaddingBorderStyle dotted_border();

    static PaddingBorderStyle dotted_thick_border();

    static PaddingBorderStyle block_border();

    static PaddingBorderStyle of_char(char16_t character);

private:
    explicit PaddingBorderStyle(char16_t left, char16_t top, char16_t right, char16_t bottom,
                                char16_t top_left_corner, char16_t top_right_corner,
                                char16_t bottom_right_corner, char16_t bottom_left_corner);
};
