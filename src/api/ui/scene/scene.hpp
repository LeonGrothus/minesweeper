#pragma once

class Scene {
public:
	virtual ~Scene();

	virtual bool is_dirty();

	virtual void keyboard_press(int key);

	virtual void update(double delta_time);

	virtual void draw();
};