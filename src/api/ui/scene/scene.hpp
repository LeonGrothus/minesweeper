#pragma once
#include "api/ui/widget/widget.hpp"
#include <memory>

class Scene {
public:
	virtual ~Scene() = 0;

	virtual bool is_dirty() {
		return m_base_widget->is_dirty();
	}

	virtual void keyboard_press(const int key) {
		m_base_widget->keyboard_press(key);
	}

	virtual void update(const double delta_time) {
		m_base_widget->update(delta_time);
	}

	virtual const CanvasElement &build_scene(const ElementSize &size) {
		return m_base_widget->build_widget(size);
	}

protected:
	std::unique_ptr<Widget> m_base_widget;
};

inline Scene::~Scene() = default;
