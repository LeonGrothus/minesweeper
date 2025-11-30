#include "grid_2d.hpp"


template <typename T>
Grid2D<T>::Grid2D(const Vector2D size, const Vector2D offset) : m_size(size), m_offset(offset),
                                                                m_data(new T[size.x * size.y]) {
}

template <typename T>
int Grid2D<T>::get_index(const Vector2D pos) const {
	return pos.x + pos.y * m_size.x;
}

template <typename T>
const T &Grid2D<T>::get_at_index(const int index) {
	return m_data[index];
}

template <typename T>
Vector2D Grid2D<T>::get_index_position(const int index) const {
	return Vector2D(index % m_size.x + m_offset.x, index / m_size.x + m_offset.y);
}

template <typename T>
Vector2D Grid2D<T>::modulo_position(const Vector2D pos) const {
	return pos % m_size;
}

template <typename T>
void Grid2D<T>::set_at_index(const int index, const T value) {
	m_data[index] = value;
}

template <typename T>
bool Grid2D<T>::in_bounds(const int index) const {
	return (get_index_position(index) - m_offset) < m_size;
}

template <typename T>
bool Grid2D<T>::in_bounds(const Vector2D pos) const {
	return (pos - m_offset) < m_size;
}

template <typename T>
std::vector<std::tuple<Vector2D, T> > Grid2D<T>::get_close_adjacent(const Vector2D pos) {
	std::vector<std::tuple<Vector2D, T> > adjacent;
	for (int local_x = -1; local_x <= 1; local_x++) {
		for (int local_y = -1; local_y <= 1; local_y++) {
			if (abs(local_x + local_y) != 1) {
				continue;
			}
			const Vector2D new_pos = Vector2D{pos.x + local_x, pos.y + local_y};
			if (!in_bounds(new_pos)) {
				continue;
			}
			adjacent.push_back(std::tuple<Vector2D, int>(new_pos + m_offset, m_data[get_index(new_pos)]));
		}
	}
	return adjacent;
}

template <typename T>
std::vector<T> Grid2D<T>::get_all_adjacent(const Vector2D pos) {
	std::vector<std::tuple<Vector2D, T> > adjacent;
	for (int local_x = -1; local_x <= 1; local_x++) {
		for (int local_y = -1; local_y <= 1; local_y++) {
			if (local_x + local_y == 0) {
				continue;
			}
			const Vector2D new_pos = Vector2D{pos.x + local_x, pos.y + local_y};
			if (!in_bounds(new_pos)) {
				continue;
			}
			adjacent.push_back(std::tuple<Vector2D, int>(new_pos + m_offset, m_data[get_index(new_pos)]));
		}
	}
	return adjacent;
}

template <typename T>
T &Grid2D<T>::operator[](const Vector2D pos) {
	return m_data[get_index(pos - m_offset)];
}

template <typename T>
T &Grid2D<T>::operator[](const int x, const int y) {
	return this[Vector2D(x, y)];
}