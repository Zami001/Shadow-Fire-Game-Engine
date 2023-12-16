#include <Profiling/TimedProfiler.h>

#include <Logging/Log.h>

TimedProfiler::TimedProfiler(const char* task) : taskname(task), counter(0) {
	timer = std::chrono::steady_clock::now();
}


void TimedProfiler::Step() {
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timer).count() >= 1000) {
		SF_LOG(Timed Profiler, Profiling, "%s: %i", taskname, counter)
		timer = std::chrono::steady_clock::now();
		counter = 0;
	} else {
		counter++;
	}
}