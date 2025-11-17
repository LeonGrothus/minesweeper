#pragma once

class Scene {
public:
	virtual ~Scene() = 0;

	virtual bool is_dirty() = 0;

	virtual void keyboard_press(int key) = 0;

	virtual bool update(double delta_time) = 0;

	virtual void draw() = 0;
};

inline Scene::~Scene() = default;
