#include "border_style.hpp"

BorderStyle BorderStyle::double_line_border() {
    return BorderStyle(u'║', u'═', u'║', u'═', u'╔', u'╗', u'╝', u'╚');
}

BorderStyle BorderStyle::single_line_border() {
    return BorderStyle(u'│', u'─', u'│', u'─', u'┌', u'┐', u'┘', u'└');
}

BorderStyle BorderStyle::single_thick_border() {
    return BorderStyle(u'┃', u'━', u'┃', u'━', u'┏', u'┓', u'┛', u'┗');
}

BorderStyle BorderStyle::dotted_border() {
    return BorderStyle(u'┆', u'┄', u'┆', u'┄', u'┌', u'┐', u'┘', u'└');
}

BorderStyle BorderStyle::dotted_thick_border() {
    return BorderStyle(u'┇', u'┅', u'┇', u'┅', u'┏', u'┓', u'┛', u'┗');
}

BorderStyle BorderStyle::block_border() {
    return BorderStyle(u'█', u'█', u'█', u'█', u'█', u'█', u'█', u'█');
}
BorderStyle BorderStyle::of_char(const char16_t character) {
    return BorderStyle(character, character, character, character, character, character, character, character);
}


BorderStyle::BorderStyle(const char16_t left, const char16_t top, const char16_t right, const char16_t bottom,
                                       const char16_t top_left_corner,
                                       const char16_t top_right_corner, const char16_t bottom_right_corner,
                                       const char16_t bottom_left_corner)
    : left(left), top(top), right(right), bottom(bottom),
      top_left_corner(top_left_corner), top_right_corner(top_right_corner),
      bottom_right_corner(bottom_right_corner), bottom_left_corner(bottom_left_corner) {
}
