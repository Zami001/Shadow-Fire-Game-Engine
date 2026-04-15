#pragma once

#include <Core.h>
#include <Logging/LogManager.h>

#ifndef LOG_LEVEL
	#define LOG_LEVEL LogSeverity::VeryVerbose
#endif

#define SF_LOG(Category, Severity, Message, ...) if constexpr (LogSeverity::##Severity >= LOG_LEVEL) { LogManager::Get().LogMessage(LogSeverity::##Severity, #Category, Message, __VA_ARGS__); }