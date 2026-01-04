#pragma once
#include "../canvas/canvas_element.hpp"
#include "api/ui/canvas/terminal_helper.hpp"

enum Flex {
    NO_FLEX = 0,
    DEFAULT_FLEX = 1
};

class Widget {
public:
    virtual ~Widget() = 0;

    const CanvasElement &build_widget(const Vector2D &size) {
        if (size < get_minimum_size()) {
            show_temporary_message("INCREASE SCREEN SIZE", 200);
            if (m_cached_canvas.get_element_size() != size) {
                m_cached_canvas = CanvasElement::empty(size, u' ');
            }
            return m_cached_canvas;
        }

        if (!is_dirty() && m_cached_canvas.get_element_size() == size) {
            return m_cached_canvas;
        }

        m_cached_canvas = std::move(build_canvas_element(size));

        if (m_cached_canvas.get_element_size() != size) {
            m_cached_canvas = m_cached_canvas.fill_to_size(size, u' ');
        }

        m_is_dirty = false;
        return m_cached_canvas;
    }

    virtual Vector2D get_minimum_size() const = 0;

    virtual void keyboard_press(int key) = 0;

    virtual void update(double delta_time) = 0;

    virtual bool is_dirty() const = 0;

    virtual void set_dirty() {
        m_is_dirty = true;
    }

    int m_flex = DEFAULT_FLEX;

protected:
    virtual CanvasElement build_canvas_element(const Vector2D &size) = 0;

    bool m_is_dirty = true;
    CanvasElement m_cached_canvas{CanvasElement("")};
};

inline Widget::~Widget() = default;
