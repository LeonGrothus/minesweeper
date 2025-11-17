#include "scene.hpp"

Scene::~Scene() = default;

bool Scene::is_dirty() {
	return true;
}

void Scene::keyboard_press(int key) {
}

void Scene::update(double delta_time) {
}

void Scene::draw() {
}
