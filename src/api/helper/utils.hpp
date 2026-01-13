#pragma once
#include <random>

std::mt19937& get_rng();

template <typename T, std::size_t N>
void insert_and_drop_last(std::array<T, N>& array, const size_t index, const T& value) {
    static_assert(N > 0);
    if (index >= N) {
        return;
    }
    for (size_t i = N - 1; i > index; i--) {
        array[i] = array[i - 1];
    }
    array[index] = value;
}

