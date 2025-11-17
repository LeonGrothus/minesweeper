#pragma once

#include <chrono>

class DeltaTimer {
public:
	explicit DeltaTimer();

	void update();

	void reset();

	double get_delta_seconds() const;

	double get_delta_millis() const;

	long get_delta_nanos() const;

	double get_elapsed_seconds() const;

private:
	std::chrono::steady_clock::time_point m_last;
	double m_delta_seconds;
	double m_elapsed_seconds;
};