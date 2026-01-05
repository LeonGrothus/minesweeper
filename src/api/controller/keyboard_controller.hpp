#pragma once
#include <vector>

class KeyboardController {
public:
    ~KeyboardController();

    int get_key() const;

    std::vector<int> get_buffered() const;
};
