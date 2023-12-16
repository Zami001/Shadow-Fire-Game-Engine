#pragma once

#include <Core.h>
#include <stdint.h>
#include <chrono>

class CORE_API TimedProfiler {
public:
	TimedProfiler(const char* task);
	~TimedProfiler() = default;

	void Step();

private:
	const char* taskname;
	uint32_t counter;
	std::chrono::time_point<std::chrono::steady_clock> timer;
};