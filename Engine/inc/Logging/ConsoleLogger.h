#pragma once

#include <Engine.h>
#include <Logging/LogOutput.h>
#include <Logging/Log.h>

class ENGINE_API ConsoleLogger : public LogOutput {
public:
	ConsoleLogger() = default;
	virtual ~ConsoleLogger() = default;
	
	virtual void OutputLog(LogSeverity Severity, const std::string& Category, const std::string& Message) override;
};