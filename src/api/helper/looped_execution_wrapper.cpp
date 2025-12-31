#include "looped_execution_wrapper.hpp"

#include <assert.h>

LoopedExecutionWrapper::LoopedExecutionWrapper(std::function<void()> runnable,
                                               const double time_to_repeat) : m_runnable(std::move(runnable)),
                                                                              m_time_to_repeat(time_to_repeat) {
}

void LoopedExecutionWrapper::update(const double delta_time) {
    m_current_time += delta_time;

    if (m_current_time < m_time_to_repeat) {
        return;
    }

    int repeat_count = static_cast<int>(m_current_time / m_time_to_repeat);
    m_current_time -= repeat_count * m_time_to_repeat;

    while (repeat_count-- > 0) {
        m_runnable();
    }
}

void LoopedExecutionWrapper::reset() {
    m_current_time = 0;
}

void LoopedExecutionWrapper::set_repeat_time(const double time_to_repeat) {
    assert(time_to_repeat >= 0);
    m_time_to_repeat = time_to_repeat;
    reset();
}
