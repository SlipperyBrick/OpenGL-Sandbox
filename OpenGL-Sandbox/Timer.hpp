#pragma once

#include <chrono>
#include <iostream>

enum class TIMER_ENUM {
	TIMER_SECONDS,
	TIMER_MICROSECONDS
};

class Timer {
public:



	Timer() {
		m_start = std::chrono::high_resolution_clock::now();
		m_output = 0;
		m_enum = TIMER_ENUM::TIMER_MICROSECONDS;
		m_duration.zero();
	}
	Timer(TIMER_ENUM tn)  { 
		m_start = std::chrono::high_resolution_clock::now(); 
		m_output = 0;
		m_enum = tn;
		m_duration.zero();
	};
	~Timer() { 
		m_end = std::chrono::high_resolution_clock::now();
		m_duration = m_end - m_start;

		if (m_enum == TIMER_ENUM::TIMER_MICROSECONDS) {
			m_output = m_duration.count() * 1000.f;
			printf("Timer: %.4fms\n", m_output);
		} 
		else if (m_enum == TIMER_ENUM::TIMER_SECONDS) {
			m_output = m_duration.count();
			printf("Timer: %.2fs\n", m_output);
		}
		
	};



private:
	std::chrono::time_point<std::chrono::steady_clock> m_start, m_end;
	std::chrono::duration<float> m_duration;
	float m_output;
	TIMER_ENUM m_enum;

};