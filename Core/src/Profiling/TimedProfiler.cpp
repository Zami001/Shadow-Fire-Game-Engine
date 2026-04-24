#include <Profiling/TimedProfiler.h>

#include <Logging/Log.h>

#if SF_PROFILING
TimedProfiler::TimedProfiler(const char* task) : taskname(task), counter(0) {
	timer = std::chrono::steady_clock::now();
}
#else
TimedProfiler::TimedProfiler(const char* task) {}
#endif


void TimedProfiler::Step() {
#if SF_PROFILING
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timer).count() >= 1000) {
		SF_LOG(Timed Profiler, Profiling, "%s: %i", taskname, counter)
		timer = std::chrono::steady_clock::now();
		counter = 0;
	} else {
		counter++;
	}
#endif
}