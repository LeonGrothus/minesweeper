#pragma once
#include <functional>

class LoopedExecutionWrapper {
public:
    explicit LoopedExecutionWrapper(std::function<void()> runnable, double time_to_repeat);

    void update(double delta_time);

    void reset();

    void set_repeat_time(double time_to_repeat);

private:
    std::function<void()> m_runnable;

    double m_time_to_repeat;
    double m_current_time = 0;
};

