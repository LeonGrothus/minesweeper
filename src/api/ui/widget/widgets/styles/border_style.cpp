#include "border_style.hpp"

PaddingBorderStyle PaddingBorderStyle::double_line_border() {
    return PaddingBorderStyle(u'║', u'═', u'║', u'═', u'╔', u'╗', u'╝', u'╚');
}

PaddingBorderStyle PaddingBorderStyle::single_line_border() {
    return PaddingBorderStyle(u'│', u'─', u'│', u'─', u'┌', u'┐', u'┘', u'└');
}

PaddingBorderStyle PaddingBorderStyle::single_thick_border() {
    return PaddingBorderStyle(u'┃', u'━', u'┃', u'━', u'┏', u'┓', u'┛', u'┗');
}

PaddingBorderStyle PaddingBorderStyle::dotted_border() {
    return PaddingBorderStyle(u'┆', u'┄', u'┆', u'┄', u'┌', u'┐', u'┘', u'└');
}

PaddingBorderStyle PaddingBorderStyle::dotted_thick_border() {
    return PaddingBorderStyle(u'┇', u'┅', u'┇', u'┅', u'┏', u'┓', u'┛', u'┗');
}

PaddingBorderStyle PaddingBorderStyle::block_border() {
    return PaddingBorderStyle(u'█', u'█', u'█', u'█', u'█', u'█', u'█', u'█');
}
PaddingBorderStyle PaddingBorderStyle::of_char(const char16_t character) {
    return PaddingBorderStyle(character, character, character, character, character, character, character, character);
}


PaddingBorderStyle::PaddingBorderStyle(const char16_t left, const char16_t top, const char16_t right, const char16_t bottom,
                                       const char16_t top_left_corner,
                                       const char16_t top_right_corner, const char16_t bottom_right_corner,
                                       const char16_t bottom_left_corner)
    : left(left), top(top), right(right), bottom(bottom),
      top_left_corner(top_left_corner), top_right_corner(top_right_corner),
      bottom_right_corner(bottom_right_corner), bottom_left_corner(bottom_left_corner) {
}
