#pragma once

#include <Core.h>
#include <chrono>

class CORE_API ScopedProfiler {
public:
	ScopedProfiler(const char* name);
	~ScopedProfiler();

	void operator=(const ScopedProfiler&) = delete;
	void operator=(ScopedProfiler&&) = delete;

	void Start();
	void End();

private:
	std::chrono::time_point<std::chrono::steady_clock> start;
	const char* taskname;
	bool active = true;

	void Print();
};