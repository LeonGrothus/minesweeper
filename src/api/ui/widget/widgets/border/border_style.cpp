#include "border_style.hpp"

#ifdef __linux__
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
#elifdef _WIN32
BorderStyle BorderStyle::double_line_border() {
    return BorderStyle(u'\u2551', u'\u2550', u'\u2551', u'\u2550', u'\u2554', u'\u2557', u'\u255D', u'\u255A');
}

BorderStyle BorderStyle::single_line_border() {
    return BorderStyle(u'\u2502', u'\u2500', u'\u2502', u'\u2500', u'\u250C', u'\u2510', u'\u2518', u'\u2514');
}

BorderStyle BorderStyle::single_thick_border() {
    return BorderStyle(u'\u2503', u'\u2501', u'\u2503', u'\u2501', u'\u250F', u'\u2513', u'\u251B', u'\u2517');
}

BorderStyle BorderStyle::dotted_border() {
    return BorderStyle(u'\u2506', u'\u2504', u'\u2506', u'\u2504', u'\u250C', u'\u2510', u'\u2518', u'\u2514');
}

BorderStyle BorderStyle::dotted_thick_border() {
    return BorderStyle(u'\u2507', u'\u2505', u'\u2507', u'\u2505', u'\u250F', u'\u2513', u'\u251B', u'\u2517');
}

BorderStyle BorderStyle::block_border() {
    return BorderStyle(u'\u2588', u'\u2588', u'\u2588', u'\u2588', u'\u2588', u'\u2588', u'\u2588', u'\u2588');
}
#endif

BorderStyle::BorderStyle(const char16_t left, const char16_t top, const char16_t right, const char16_t bottom,
                         const char16_t top_left_corner, const char16_t top_right_corner,
                         const char16_t bottom_right_corner, const char16_t bottom_left_corner)
    : left(left), top(top), right(right), bottom(bottom),
      top_left_corner(top_left_corner), top_right_corner(top_right_corner),
      bottom_right_corner(bottom_right_corner), bottom_left_corner(bottom_left_corner) {
}
