#include "random_helper.hpp"

std::mt19937 &get_rng() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    return rng;
}
