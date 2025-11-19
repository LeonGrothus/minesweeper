#include "delta_timer.hpp"

delta_timer::delta_timer() {
	m_delta_seconds = 0;
	m_elapsed_seconds = 0;
	m_last = std::chrono::steady_clock::now();
}

void delta_timer::update() {
	const std::chrono::time_point now = std::chrono::steady_clock::now();
	const std::chrono::duration<double> diff = now - m_last;
	m_last = now;

	m_delta_seconds = diff.count();
	m_elapsed_seconds += m_delta_seconds;
}

void delta_timer::reset() {
	m_last = std::chrono::steady_clock::now();
	m_delta_seconds = 0.0;
	m_elapsed_seconds = 0.0;
}

double delta_timer::get_delta_seconds() const {
	return m_delta_seconds;
}

double delta_timer::get_delta_millis() const {
	return m_delta_seconds * 1000.0;
}

long delta_timer::get_delta_nanos() const {
	return static_cast<long>(m_delta_seconds * 1e9);
}

double delta_timer::get_elapsed_seconds() const {
	return m_elapsed_seconds;
}