#include "terminal_helper.hpp"
#include "api/ui/canvas/canvas_element.hpp"
#include <algorithm>
#include <iostream>
#include <curses.h>
#include <string>
#include <string_view>

std::string u16_to_utf8(const std::u16string &s) {
    std::string out;
    out.reserve(s.size() * 3);
    for (size_t i = 0; i < s.size(); ++i) {
        uint32_t codepoint = s[i];
        if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
            if (i + 1 < s.size()) {
                if (const uint32_t low = s[i + 1]; low >= 0xDC00 && low <= 0xDFFF) {
                    codepoint = (((codepoint - 0xD800) << 10) | (low - 0xDC00)) + 0x10000;
                    i++;
                }
            }
        }

        if (codepoint <= 0x7F) {
            out.push_back(static_cast<char>(codepoint));
        } else if (codepoint <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else if (codepoint <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else {
            out.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
    }
    return out;
}

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
    const std::u16string half_width_offset = std::u16string((canvas_size.x - element_width) / 2, blank_char);

    std::u16string vertical_string_line;
    vertical_string_line.reserve(canvas_size.x);

    std::u16string vertical_string;
    vertical_string.reserve(static_cast<size_t>(canvas_size.x) * element_height);

    for (int i = 0; i < element_height; i++) {
        const std::u16string::size_type current_pos = static_cast<std::u16string::size_type>(i) * element_width;
        const std::u16string_view line(element.get_canvas_element().data() + current_pos, element_width);

        vertical_string_line = repeat_string(repeat_left, half_width_offset);
        vertical_string_line += line;
        vertical_string_line += repeat_string(repeat_right, half_width_offset);
        vertical_string_line += std::u16string(canvas_size.x - static_cast<int>(vertical_string_line.length()), blank_char);
        vertical_string += vertical_string_line;
    }

    std::u16string full_string;
    const int height_diff = (canvas_size.y - element_height) / 2;
    const int additional = (canvas_size.y - element_height) % 2;
    full_string.reserve(static_cast<size_t>(canvas_size.x) * canvas_size.y);

    const bool extra_at_top = (repeat_bottom != 0);

    for (int i = 0; i < repeat_top * height_diff; i++) {
        full_string += width_offset;
    }
    if (additional && extra_at_top) {
        full_string += width_offset;
    }
    full_string += vertical_string;
    for (int i = 0; i < repeat_bottom * height_diff; i++) {
        full_string += width_offset;
    }
    if (additional && !extra_at_top) {
        full_string += width_offset;
    }
    return CanvasElement(full_string, canvas_size);
}

void position_string_on_canvas(const CanvasElement &element, const Position pos, CanvasElement &canvas) {
    const Vector2D canvas_size = canvas.get_element_size();
    const int canvas_width = canvas_size.x;
    const int canvas_height = canvas_size.y;

    if (element.get_width() > canvas_width || element.get_height() > canvas_height) {
        return;
    }

    //'\x7F' is non printable delete char
    const std::u16string alpha_canvas = position_canvas_element(element, pos, canvas_size, u'\x7F').get_canvas_element();
    std::u16string &canvas_canvas = canvas.get_mutable_canvas_element();

    for (int i = 0; i < canvas_width * canvas_height; i++) {
        if (alpha_canvas[i] == u'\x7F') {
            continue;
        }
        canvas_canvas[i] = alpha_canvas[i];
    }
}

void render_to_ncurses(const std::string &to_render, const Vector2D size) {
    clear();

    for (int y = 0; y < size.y; y++) {
        const int offset = y * size.x;
        mvaddnstr(y, 0, to_render.c_str() + offset, size.x);
    }

    refresh();
}

void render_to_ncurses(const std::u16string &to_render, const Vector2D size) {
    clear();
    for (int y = 0; y < size.y; y++) {
        const int offset = y * size.x;
        const std::u16string_view line(to_render.data() + offset, size.x);
        const std::string utf8 = u16_to_utf8(std::u16string(line));
        mvaddnstr(y, 0, utf8.c_str(), static_cast<int>(utf8.size()));
    }
    refresh();
}

static std::u16string last_frame;

void render_to_ncurses_buffered(const std::string &to_render, const Vector2D size) {
    static std::string last;
    if (last.size() != to_render.size()) {
        last.assign(to_render.size(), '\x7F');
    }
    for (int i = 0; i < static_cast<int>(to_render.size()); i++) {
        if (last[i] == to_render[i]) {
            continue;
        }
        const int x = i % size.x;
        const int y = i / size.x;
        mvaddch(y, x, to_render[i]);
    }
    last = to_render;
    wnoutrefresh(stdscr);
    doupdate();
}

void render_to_ncurses_buffered(const std::u16string &to_render, const Vector2D size) {
    if (last_frame.size() != to_render.size()) {
        last_frame.assign(to_render.size(), u'\x7F');
    }

    for (int i = 0; i < static_cast<int>(to_render.size()); i++) {
        if (last_frame[i] == to_render[i]) {
            continue;
        }
        const int x = i % size.x;
        const int y = i / size.x;
        const std::string utf8 = u16_to_utf8(std::u16string(1, to_render[i]));
        mvaddnstr(y, x, utf8.c_str(), static_cast<int>(utf8.size()));
    }
    last_frame = to_render;

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
    const std::string utf8 = u16_to_utf8(message);
    show_temporary_message(utf8, duration_ms);
}
