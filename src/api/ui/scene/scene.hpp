#pragma once
#include "api/ui/widget/widget.hpp"
#include <memory>

class Scene {
public:
	virtual ~Scene() = 0;

	void request_scene_change(std::unique_ptr<Scene> next_scene) {
		m_pending_scene = std::move(next_scene);
	}

	std::unique_ptr<Scene> take_pending_scene() {
		return std::move(m_pending_scene);
	}

	virtual void set_dirty() {
		if (m_base_widget) {
			m_base_widget->set_dirty();
		}
	}

	virtual bool is_dirty() {
		return m_base_widget->is_dirty();
	}

	virtual void keyboard_press(const int key) {
		m_base_widget->keyboard_press(key);
	}

	virtual void update(const double delta_time) {
		m_base_widget->update(delta_time);
	}

	virtual const CanvasElement &build_scene(const Vector2D &size) {
		return m_base_widget->build_widget(size);
	}

protected:
	std::shared_ptr<Widget> m_base_widget;
	std::unique_ptr<Scene> m_pending_scene;
};

inline Scene::~Scene() = default;
