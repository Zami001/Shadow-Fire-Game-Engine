#pragma once

#include <Core.h>
#include <string>

enum class LogSeverity : unsigned char {
	VeryVerbose,
	Verbose,
	Profiling,
	Log,
	Warning,
	Error,
	Fatal
};

class CORE_API LogOutput {
public:
	LogOutput();
	virtual ~LogOutput();

	virtual void OutputLog(LogSeverity Severity, const std::string& Category, const std::string& Message) = 0;
};