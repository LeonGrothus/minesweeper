#pragma once
#include "api/ui/canvas/canvas_element.hpp"

#include <vector>

template <typename T>
class Grid2D {

public:
	explicit Grid2D(Vector2D size, Vector2D offset = {0, 0});

	int get_index(Vector2D pos) const;

	const T &get_at_index(int index);

	Vector2D get_index_position(int index) const;

	Vector2D modulo_position(Vector2D pos) const;

	void set_at_index(int index, T value);

	bool in_bounds(int index) const;

	bool in_bounds(Vector2D pos) const;

	std::vector<std::tuple<Vector2D, T> > get_close_adjacent(Vector2D pos);

	std::vector<T> get_all_adjacent(Vector2D pos);

	T &operator [](Vector2D pos);

	T &operator[](int x, int y);

private:
	Vector2D m_size;
	Vector2D m_offset;
	T *m_data;
};