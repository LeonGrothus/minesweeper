#include "conversion_helper.hpp"

#include <cstdint>
#include <string>

//
// this code is from stackoverflow with small modifications
//

std::string utf16_to_utf8(const std::u16string &input) {
    std::string out;
    out.reserve(input.size() * 3);

    for (size_t i = 0; i < input.size(); ++i) {
        uint32_t code = input[i];

        // surrogate handling
        if (0xD800 <= code && code <= 0xDBFF) {
            // high surrogate
            if (i + 1 < input.size()) {
                if (const uint32_t low = input[i + 1]; 0xDC00 <= low && low <= 0xDFFF) {
                    code = (((code - 0xD800) << 10) | (low - 0xDC00)) + 0x10000;
                    ++i;
                } else {
                    code = 0xFFFD; // unpaired high surrogate
                }
            } else {
                code = 0xFFFD; // high surrogate at end
            }
        } else if (0xDC00 <= code && code <= 0xDFFF) {
            code = 0xFFFD; // unpaired low surrogate
        }

        // encode UTF-8
        if (code <= 0x7F) {
            out.push_back(static_cast<char>(code));
        } else if (code <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | (code >> 6)));
            out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
        } else if (code <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | (code >> 12)));
            out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
        } else {
            out.push_back(static_cast<char>(0xF0 | (code >> 18)));
            out.push_back(static_cast<char>(0x80 | ((code >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
        }
    }

    return out;
}

std::u16string utf8_to_utf16(const std::string &input) {
    std::u16string out;
    out.reserve(input.size());

    size_t i = 0;
    while (i < input.size()) {
        const auto c = static_cast<uint8_t>(input[i]);

        uint32_t codepoint = 0;
        size_t extra = 0;

        if (c <= 0x7F) {
            codepoint = c;
            extra = 0;
        } else if ((c & 0xE0) == 0xC0) {
            codepoint = c & 0x1F;
            extra = 1;
        } else if ((c & 0xF0) == 0xE0) {
            codepoint = c & 0x0F;
            extra = 2;
        } else if ((c & 0xF8) == 0xF0) {
            codepoint = c & 0x07;
            extra = 3;
        } else {
            // invalid prefix byte
            out.push_back(u'\uFFFD');
            ++i;
            continue;
        }

        if (i + extra >= input.size()) {
            // truncated sequence
            out.push_back(u'\uFFFD');
            break;
        }

        bool valid = true;
        for (size_t j = 1; j <= extra; ++j) {
            const auto cc = static_cast<uint8_t>(input[i + j]);
            if ((cc & 0xC0) != 0x80) {
                valid = false;
                break;
            }
            codepoint = (codepoint << 6) | (cc & 0x3F);
        }

        if (!valid) {
            out.push_back(u'\uFFFD');
            i += 1;
            continue;
        }

        i += extra + 1;

        // UTF-8 overlong protection
        static constexpr uint32_t min_val[4] = {0, 0, 0x80, 0x800};
        if (codepoint < min_val[extra + 1]) {
            out.push_back(u'\uFFFD');
            continue;
        }

        // UTF-16 encoding
        if (codepoint <= 0xFFFF) {
            if (0xD800 <= codepoint && codepoint <= 0xDFFF) {
                out.push_back(u'\uFFFD');
            } else {
                out.push_back(static_cast<char16_t>(codepoint));
            }
        } else {
            codepoint -= 0x10000;
            out.push_back(static_cast<char16_t>(0xD800 + (codepoint >> 10)));
            out.push_back(static_cast<char16_t>(0xDC00 + (codepoint & 0x3FF)));
        }
    }

    return out;
}
