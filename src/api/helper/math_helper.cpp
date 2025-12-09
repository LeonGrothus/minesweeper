#include "math_helper.hpp"

int digits(const int value) {
    int result = 0;
    int v = value;
    do {
        result++;
        v /= 10;
    } while (v > 0);
    return result;
}
