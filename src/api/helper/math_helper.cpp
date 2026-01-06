#include "math_helper.hpp"

#include <cmath>

int digits(const int value) {
    int result = 0;
    int v = value;
    do {
        result++;
        v /= 10;
    } while (v > 0);
    return result;
}

double deg_to_rad(const double degrees) {
    return degrees * (M_PI / 180);
}

double rad_to_deg(const double radians) {
    return radians * (180 / M_PI);
}

Vector2D rotate_around(const Vector2D to_rotate, const double radians) {
    const double x1 = cos(radians);
    const double x2 = -sin(radians);
    const double x3 = -x2;

    const int new_x = static_cast<int>(lround(to_rotate.x * x1 + to_rotate.y * x2));
    const int new_y = static_cast<int>(lround(to_rotate.x * x3 + to_rotate.y * x1));

    return Vector2D{new_x, new_y};
}
