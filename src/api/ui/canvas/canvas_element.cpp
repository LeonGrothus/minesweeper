#include "canvas_element.hpp"

#include <bitset>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <string_view>
#include "api/helper/conversion_helper.hpp"
#include "api/ui/widget/widgets/border/border_style.hpp"

CanvasElement::CanvasElement(std::u16string canvas_element, const Vector2D size, const ColorRole role) {
    m_size = size;
    m_color_roles.assign(m_size.area(), static_cast<uint8_t>(role));
    m_canvas_element = std::move(canvas_element);
}

CanvasElement::CanvasElement(std::u16string canvas_element, const int width, const int height, const ColorRole role) {
    m_size = Vector2D{width, height};
    m_color_roles.assign(m_size.area(), static_cast<uint8_t>(role));
    m_canvas_element = std::move(canvas_element);
}

CanvasElement::CanvasElement(std::u16string canvas_element, std::vector<uint8_t> color_roles, const Vector2D size) {
    m_size = size;
    m_canvas_element = std::move(canvas_element);
    m_color_roles = std::move(color_roles);
}

CanvasElement::CanvasElement(const std::string &normal_string, const char delimiter, const ColorRole role) {
    const CanvasElement canvas = transform_to_canvas_element_utf8(normal_string, delimiter, u' ');
    m_canvas_element = canvas.get_canvas_element();
    m_size = canvas.m_size;
    m_color_roles.assign(m_size.area(), static_cast<uint8_t>(role));
}

CanvasElement CanvasElement::empty(const Vector2D size, const char16_t empty_char) {
    return CanvasElement(std::u16string(size.area(), empty_char), size, ColorRole::Default);
}

int CanvasElement::get_width() const {
    return m_size.x;
}

int CanvasElement::get_height() const {
    return m_size.y;
}

Vector2D CanvasElement::get_element_size() const {
    return m_size;
}

int CanvasElement::get_total_length() const {
    return m_size.area();
}


const std::u16string &CanvasElement::get_canvas_element() const {
    return m_canvas_element;
}

std::u16string &CanvasElement::get_mutable_canvas_element() {
    return m_canvas_element;
}

const std::vector<uint8_t> &CanvasElement::get_color_roles() const {
    return m_color_roles;
}

std::vector<uint8_t> &CanvasElement::get_mutable_color_roles() {
    return m_color_roles;
}

CanvasElement CanvasElement::fill_to_size(const Vector2D size, const char16_t fill_char) const {
    if (m_size > size) {
        return CanvasElement(m_canvas_element, m_color_roles, m_size);
    }

    std::u16string new_canvas_element;
    std::vector<uint8_t> new_color_roles;
    new_canvas_element.reserve(size.area());
    new_color_roles.reserve(size.area());

    for (int i = 0; i < m_size.y; i++) {
        const std::u16string_view line(m_canvas_element.data() + i * m_size.x, m_size.x);
        const std::vector<uint8_t> line_roles(m_color_roles.begin() + i * m_size.x, m_color_roles.begin() + (i + 1) * m_size.x);
        new_canvas_element += line;
        new_color_roles.insert(new_color_roles.end(), line_roles.begin(), line_roles.end());
        new_canvas_element.append(size.x - m_size.x, fill_char);
        new_color_roles.insert(new_color_roles.end(), size.x - m_size.x, static_cast<uint8_t>(ColorRole::Default));
    }

    new_canvas_element.append((size.y - m_size.y) * size.x, fill_char);
    new_color_roles.insert(new_color_roles.end(), (size.y - m_size.y) * size.x, static_cast<uint8_t>(ColorRole::Default));

    return CanvasElement(std::move(new_canvas_element), std::move(new_color_roles), size);
}

std::ostream &operator<<(std::ostream &os, const CanvasElement &elem) {
    for (const char16_t wide_char: elem.m_canvas_element) {
        const char c = (wide_char <= 0x7F) ? static_cast<char>(wide_char) : '?';
        os << c;
    }
    return os;
}

bool CanvasElement::merge_below_with_other(const CanvasElement &other) {
    if (m_size.x != other.get_width()) {
        return false;
    }
    m_canvas_element += other.get_canvas_element();
    m_color_roles.insert(m_color_roles.end(), other.get_color_roles().begin(), other.get_color_roles().end());
    m_size.y += other.get_height();
    return true;
}

bool CanvasElement::merge_above_with_other(const CanvasElement &other) {
    if (m_size.x != other.get_width()) {
        return false;
    }
    std::u16string combined;
    combined.reserve((m_size.y + other.get_height()) * m_size.x);
    std::vector<uint8_t> combined_roles;
    combined_roles.reserve((m_size.y + other.get_height()) * m_size.x);

    combined += other.get_canvas_element();
    combined_roles.insert(combined_roles.end(), other.get_color_roles().begin(), other.get_color_roles().end());

    combined += m_canvas_element;
    combined_roles.insert(combined_roles.end(), m_color_roles.begin(), m_color_roles.end());

    m_canvas_element = std::move(combined);
    m_color_roles = std::move(combined_roles);
    m_size.y += other.get_height();
    return true;
}

bool CanvasElement::merge_right_with_other(const CanvasElement &other) {
    if (m_size.y != other.get_height()) {
        return false;
    }

    std::u16string result;
    std::vector<uint8_t> merged_roles;
    result.reserve((m_size.x + other.get_width()) * m_size.y);
    merged_roles.reserve((m_size.x + other.get_width()) * m_size.y);

    for (int i = 0; i < m_size.y; i++) {
        const std::u16string_view first(m_canvas_element.data() + i * m_size.x, m_size.x);
        const std::u16string_view second(other.get_canvas_element().data() + i * other.get_width(), other.get_width());
        result += first;
        result += second;

        merged_roles.insert(merged_roles.end(), m_color_roles.begin() + i * m_size.x, m_color_roles.begin() + (i + 1) * m_size.x);
        merged_roles.insert(merged_roles.end(), other.get_color_roles().begin() + i * other.get_width(),
                            other.get_color_roles().begin() + (i + 1) * other.get_width());
    }

    m_canvas_element = result;
    m_color_roles = merged_roles;
    m_size.x += other.get_width();
    return true;
}

bool CanvasElement::merge_left_with_other(const CanvasElement &other) {
    if (m_size.y != other.get_height()) {
        return false;
    }

    std::u16string result;
    std::vector<uint8_t> merged_roles;
    result.reserve((m_size.x + other.get_width()) * m_size.y);
    merged_roles.reserve((m_size.x + other.get_width()) * m_size.y);

    for (int i = 0; i < m_size.y; i++) {
        const std::u16string_view first(m_canvas_element.data() + i * m_size.x, m_size.x);
        const std::u16string_view second(other.get_canvas_element().data() + i * other.get_width(), other.get_width());
        result += second;
        result += first;

        merged_roles.insert(merged_roles.end(), other.get_color_roles().begin() + i * other.get_width(),
                            other.get_color_roles().begin() + (i + 1) * other.get_width());
        merged_roles.insert(merged_roles.end(), m_color_roles.begin() + i * m_size.x, m_color_roles.begin() + (i + 1) * m_size.x);
    }

    m_canvas_element = result;
    m_color_roles = merged_roles;
    m_size.x += other.get_width();
    return true;
}

CanvasElement CanvasElement::transform_to_canvas_element_utf8(const std::string &to_canvas_element, const char delimiter,
                                                              const char16_t fill_char) {
    if (to_canvas_element.empty()) {
        constexpr Vector2D size(0, 0);
        return CanvasElement(std::u16string(), size);
    }

    std::vector<std::u16string> lines;
    std::string::size_type longest = 0;

    std::string::size_type prev_pos = 0;
    std::string::size_type current_pos;

    while ((current_pos = to_canvas_element.find(delimiter, prev_pos)) != std::string::npos) {
        std::string_view line_view(to_canvas_element.data() + prev_pos, current_pos - prev_pos);
        const std::u16string line_u16 = utf8_to_utf16(std::string(line_view));
        lines.push_back(line_u16);
        longest = std::max(longest, line_u16.length());
        prev_pos = current_pos + 1;
    }

    const std::string_view last_line_view(to_canvas_element.data() + prev_pos, to_canvas_element.length() - prev_pos);
    const std::u16string last_line_u16 = utf8_to_utf16(std::string(last_line_view));
    lines.push_back(last_line_u16);
    longest = std::max(longest, last_line_u16.length());

    const int box_height = static_cast<int>(lines.size());
    const int box_width = static_cast<int>(longest);

    std::u16string result;
    result.reserve(box_width * box_height);
    std::vector<uint8_t> roles;
    roles.reserve(box_width * box_height);
    const Vector2D size(box_width, box_height);

    for (const std::u16string &line: lines) {
        result += line;
        result.append(longest - line.length(), fill_char);
        roles.insert(roles.end(), line.size(), static_cast<uint8_t>(ColorRole::Text));
        roles.insert(roles.end(), longest - line.length(), static_cast<uint8_t>(ColorRole::Default));
    }

    return CanvasElement(result, roles, size);
}
