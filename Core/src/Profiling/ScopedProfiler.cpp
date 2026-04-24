#include <Profiling/ScopedProfiler.h>

#include <Logging/Log.h>

#if SF_PROFILING
ScopedProfiler::ScopedProfiler(const char* name) : taskname(name), active(true), start(std::chrono::steady_clock::now()) {}
#else
ScopedProfiler::ScopedProfiler(const char* name) {}
#endif

ScopedProfiler::~ScopedProfiler() {
#if SF_PROFILING
	if (active) {
		Print();
	}
#endif
}

void ScopedProfiler::Start() {
#if SF_PROFILING
	start = std::chrono::steady_clock::now();
	active = true;
#endif
}

void ScopedProfiler::End() {
#if SF_PROFILING
	if (active) {
		Print();
		active = false;
	}
#endif
}

#if SF_PROFILING
void ScopedProfiler::Print() {
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
	SF_LOG(LogScopedProfiling, Profiling, "%s took %i microseconds", taskname, duration.count())
}
#endif