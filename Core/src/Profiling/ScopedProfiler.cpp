#include <Profiling/ScopedProfiler.h>

#include <Logging/Log.h>

ScopedProfiler::ScopedProfiler(const char* name) : taskname(name), active(true), start(std::chrono::steady_clock::now()) {}

ScopedProfiler::~ScopedProfiler() {
	if (active) {
		Print();
	}
}

void ScopedProfiler::Start() {
	start = std::chrono::steady_clock::now();
	active = true;
}

void ScopedProfiler::End() {
	if (active) {
		Print();
		active = false;
	}
}

void ScopedProfiler::Print() {
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
	SF_LOG(Scoped Profiling, Profiling, "%s took %i microseconds", taskname, duration.count())
}