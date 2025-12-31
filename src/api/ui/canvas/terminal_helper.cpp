#include "terminal_helper.hpp"
#include "api/ui/canvas/canvas_element.hpp"
#include <array>
#include <curses.h>
#include <string>
#include <string_view>

#include "color_manager.hpp"
#include "api/helper/conversion_helper.hpp"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#include <unistd.h>
#endif

Vector2D get_terminal_size() {
    if (stdscr != nullptr) {
        return Vector2D{COLS, LINES};
    }
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info);
    width = (int) (console_info.srWindow.Right - console_info.srWindow.Left + 1);
    height = (int) (console_info.srWindow.Bottom - console_info.srWindow.Top + 1);
    return Vector2D{width, height};
#elif defined(__linux__)
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return Vector2D{(w.ws_col - 40), w.ws_row};
#endif
}

std::u16string repeat_string(const unsigned int k, const std::u16string &s) {
    if (k == 0) {
        return u"";
    }
    std::u16string t;
    t.reserve(s.size() * k);
    for (unsigned int i = 0; i < k; i++) {
        t += s;
    }
    return t;
}

CanvasElement position_canvas_element(const CanvasElement &element, const Position position,
                                      const Vector2D canvas_size, const char16_t blank_char) {
    const int element_width = element.get_width();
    const int element_height = element.get_height();

    if (element.get_element_size() > canvas_size)
        return CanvasElement::empty(canvas_size, L' ');
    if (element.get_element_size() == canvas_size)
        return element;

    const int repeat_left = (position) & 0b11;
    const int repeat_right = (position >> 2) & 0b11;
    const int repeat_top = (position >> 4) & 0b11;
    const int repeat_bottom = (position >> 6) & 0b11;

    const std::u16string width_offset = std::u16string(canvas_size.x, blank_char);
    const std::vector<uint8_t> width_roles(static_cast<size_t>(canvas_size.x), static_cast<uint8_t>(ColorRole::Default));
    const std::u16string half_width_offset = std::u16string((canvas_size.x - element_width) / 2, blank_char);
    const std::vector<uint8_t> half_roles(half_width_offset.size(), static_cast<uint8_t>(ColorRole::Default));

    std::u16string vertical_string;
    std::vector<uint8_t> vertical_roles;
    vertical_string.reserve(static_cast<size_t>(canvas_size.x) * element_height);
    vertical_roles.reserve(static_cast<size_t>(canvas_size.x) * element_height);

    const auto &element_roles = element.get_color_roles();

    for (int i = 0; i < element_height; i++) {
        std::u16string line;
        std::vector<uint8_t> line_roles;
        line.reserve(canvas_size.x);
        line_roles.reserve(canvas_size.x);

        line.append(repeat_left * half_width_offset.size(), blank_char);
        line_roles.insert(line_roles.end(), repeat_left * half_roles.size(), static_cast<uint8_t>(ColorRole::Default));

        const auto current_pos = static_cast<std::u16string::size_type>(i) * element_width;
        const std::u16string_view text_line(element.get_canvas_element().data() + current_pos, element_width);
        line.append(text_line);
        line_roles.insert(line_roles.end(), element_roles.begin() + current_pos, element_roles.begin() + current_pos + element_width);

        line.append(repeat_right * half_width_offset.size(), blank_char);
        line_roles.insert(line_roles.end(), repeat_right * half_roles.size(), static_cast<uint8_t>(ColorRole::Default));

        if (static_cast<int>(line.length()) < canvas_size.x) {
            const int remaining = canvas_size.x - static_cast<int>(line.length());
            line.append(remaining, blank_char);
            line_roles.insert(line_roles.end(), remaining, static_cast<uint8_t>(ColorRole::Default));
        }

        vertical_string += line;
        vertical_roles.insert(vertical_roles.end(), line_roles.begin(), line_roles.end());
    }

    std::u16string full_string;
    std::vector<uint8_t> full_roles;
    const int height_diff = (canvas_size.y - element_height) / 2;
    const int additional = (canvas_size.y - element_height) % 2;
    full_string.reserve(static_cast<size_t>(canvas_size.x) * canvas_size.y);
    full_roles.reserve(static_cast<size_t>(canvas_size.x) * canvas_size.y);

    const bool extra_at_top = (repeat_bottom == 0);

    for (int i = 0; i < repeat_top * height_diff; i++) {
        full_string += width_offset;
        full_roles.insert(full_roles.end(), width_roles.begin(), width_roles.end());
    }
    if (additional && extra_at_top) {
        full_string += width_offset;
        full_roles.insert(full_roles.end(), width_roles.begin(), width_roles.end());
    }
    full_string += vertical_string;
    full_roles.insert(full_roles.end(), vertical_roles.begin(), vertical_roles.end());
    for (int i = 0; i < repeat_bottom * height_diff; i++) {
        full_string += width_offset;
        full_roles.insert(full_roles.end(), width_roles.begin(), width_roles.end());
    }
    if (additional && !extra_at_top) {
        full_string += width_offset;
        full_roles.insert(full_roles.end(), width_roles.begin(), width_roles.end());
    }
    return CanvasElement(full_string, full_roles, canvas_size);
}

void position_string_on_canvas(const CanvasElement &element, const Position pos, CanvasElement &canvas) {
    const Vector2D canvas_size = canvas.get_element_size();
    const int canvas_width = canvas_size.x;
    const int canvas_height = canvas_size.y;

    if (element.get_width() > canvas_width || element.get_height() > canvas_height) {
        return;
    }

    //'\x7F' is non printable delete char
    const CanvasElement alpha_canvas = position_canvas_element(element, pos, canvas_size, u'\x7F');
    const std::u16string &alpha_chars = alpha_canvas.get_canvas_element();
    const std::vector<uint8_t> &alpha_roles = alpha_canvas.get_color_roles();
    std::u16string &canvas_canvas = canvas.get_mutable_canvas_element();
    std::vector<uint8_t> &canvas_roles = canvas.get_mutable_color_roles();

    for (int i = 0; i < canvas_width * canvas_height; i++) {
        if (alpha_chars[i] == u'\x7F') {
            continue;
        }
        canvas_canvas[i] = alpha_chars[i];
        canvas_roles[i] = alpha_roles[i];
    }
}

// void render_to_ncurses(const std::u16string &to_render, const Vector2D size) {
//     clear();
//     for (int y = 0; y < size.y; y++) {
//         const int offset = y * size.x;
//         const std::u16string_view line(to_render.data() + offset, size.x);
//         const std::string utf8 = utf16_to_utf8(std::u16string(line));
//         mvaddnstr(y, 0, utf8.c_str(), static_cast<int>(utf8.size()));
//     }
//     refresh();
// }

void render_to_ncurses(const CanvasElement &element, const Vector2D size) {
    clear();
    const std::u16string &chars = element.get_canvas_element();
    const std::vector<uint8_t> &roles = element.get_color_roles();
    const auto [render_size_x, render_size_y] = element.get_element_size();

    short current_color = -1;
    bool color_active = false;

    for (int y = 0; y < render_size_y; y++) {
        for (int x = 0; x < render_size_x; x++) {
            const int idx = y * render_size_x + x;

            if (const short color = get_color_for_role(roles[idx]); color != current_color) {
                if (color_active) {
                    attroff(COLOR_PAIR(current_color));
                }
                attron(COLOR_PAIR(color));
                current_color = color;
                color_active = true;
            }

            const std::string utf8 = utf16_to_utf8(std::u16string(1, chars[idx]));
            mvaddnstr(y, x, utf8.c_str(), static_cast<int>(utf8.size()));
        }
    }

    if (color_active) {
        attroff(COLOR_PAIR(current_color));
    }

    refresh();
}

static std::u16string last_frame;
static std::vector<uint8_t> last_roles;

void render_to_ncurses_buffered(const CanvasElement &element, const Vector2D size) {
    const std::u16string &chars = element.get_canvas_element();
    const std::vector<uint8_t> &roles = element.get_color_roles();
    const auto [render_size_x, render_size_y] = element.get_element_size();

    const size_t total_size = render_size_x * render_size_y;

    if (last_frame.size() != total_size) {
        last_frame.assign(total_size, u'\x7F');
        last_roles.assign(total_size, 0xFF);
    }

    short current_color = -1;
    bool color_active = false;

    for (int i = 0; i < static_cast<int>(total_size); i++) {
        if (last_frame[i] == chars[i] && last_roles[i] == roles[i]) {
            if (color_active) {
                attroff(COLOR_PAIR(current_color));
                color_active = false;
            }
            continue;
        }

        const int x = i % render_size_x;
        const int y = i / render_size_x;

        if (const short color = get_color_for_role(roles[i]); color != current_color) {
            if (color_active) {
                attroff(COLOR_PAIR(current_color));
            }
            attron(COLOR_PAIR(color));
            current_color = color;
            color_active = true;
        }

        const std::string utf8 = utf16_to_utf8(std::u16string(1, chars[i]));
        mvaddnstr(y, x, utf8.c_str(), static_cast<int>(utf8.size()));
    }

    if (color_active) {
        attroff(COLOR_PAIR(current_color));
    }

    last_frame = chars;
    last_roles = roles;

    wnoutrefresh(stdscr);
    doupdate();
}

void show_temporary_message(const std::string &message, const int duration_ms) {
    auto [width, height] = get_terminal_size();

    clear();
    const int y = height / 2;
    int x = (width - static_cast<int>(message.length())) / 2;
    if (x < 0) {
        x = 0;
    }
    mvaddnstr(y, x, message.c_str(), static_cast<int>(message.size()));
    refresh();

    napms(duration_ms);
}

void show_temporary_message(const std::u16string &message, const int duration_ms) {
    const std::string utf8 = utf16_to_utf8(message);
    show_temporary_message(utf8, duration_ms);
}
