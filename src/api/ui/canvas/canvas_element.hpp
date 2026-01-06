#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct BorderStyle;

enum class TextAlignment : uint8_t {
    Left = 0b00000010, // 0 left, 2 right
    Center = 0b00000101, // 1 left, 1 right
    Right = 0b00001000 // 2 left, 0 right
};

struct Vector2D {
    int x;
    int y;

    auto operator<=>(const Vector2D &a) const = default;

    bool operator<(const Vector2D &other) const {
        return x < other.x || y < other.y;
    }

    bool operator<=(const Vector2D &other) const {
        return x <= other.x || y <= other.y;
    }

    bool operator>(const Vector2D &other) const {
        return x > other.x || y > other.y;
    }

    bool operator>=(const Vector2D &other) const {
        return x >= other.x || y >= other.y;
    }

    Vector2D operator +(const Vector2D &other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator -(const Vector2D &other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator/(const int i) const {
        return Vector2D{x / i, y / i};
    }

    Vector2D operator %(const Vector2D &other) const {
        return Vector2D(x % other.x, y % other.y);
    }

    int area() const {
        return x * y;
    }
};

//are set in color_manager
enum class ColorRole : uint8_t {
    Default = 0,
    Hidden,
    Mine,
    Flag,
    Cursor,
    Number1,
    Number2,
    Number3,
    Number4,
    Number5,
    Number6,
    Number7,
    Number8,
    Text,
    Transition,
    Count
};

class CanvasElement {
public:
    explicit CanvasElement(std::u16string canvas_element, Vector2D size, ColorRole role = ColorRole::Text);

    explicit CanvasElement(std::u16string canvas_element, int width, int height, ColorRole role = ColorRole::Text);

    explicit CanvasElement(std::u16string canvas_element, std::vector<uint8_t> color_roles, Vector2D size);

    explicit CanvasElement(const std::string &normal_string, char delimiter = '\n', ColorRole role = ColorRole::Text,
                           TextAlignment alignment = TextAlignment::Left);

    explicit CanvasElement(const std::u16string &normal_string, char16_t delimiter = u'\n', ColorRole role = ColorRole::Text,
                           TextAlignment alignment = TextAlignment::Left);

    static CanvasElement empty(Vector2D size, char16_t empty_char);

    int get_width() const;

    int get_height() const;

    Vector2D get_element_size() const;

    int get_total_length() const;

    const std::u16string &get_canvas_element() const;

    std::u16string &get_mutable_canvas_element();

    const std::vector<uint8_t> &get_color_roles() const;

    std::vector<uint8_t> &get_mutable_color_roles();

    CanvasElement fill_to_size(Vector2D size, char16_t fill_char) const;

    bool merge_below_with_other(const CanvasElement &other);

    bool merge_above_with_other(const CanvasElement &other);

    bool merge_right_with_other(const CanvasElement &other);

    bool merge_left_with_other(const CanvasElement &other);

    friend std::ostream &operator<<(std::ostream &os, const CanvasElement &elem);

private:
    static CanvasElement transform_to_canvas_element(const std::u16string &to_canvas_element, char16_t delimiter,
                                                     char16_t fill_char, TextAlignment alignment);

    Vector2D m_size{};

    std::u16string m_canvas_element;
    std::vector<uint8_t> m_color_roles;
};
