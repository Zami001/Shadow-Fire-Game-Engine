#pragma once

#include <Core.h>
#include <Logging/LogOutput.h>
#include <vector>
#include <unordered_set>

class CORE_API LogManager {
	friend LogOutput;

private:
	LogManager() = default;
	~LogManager() = default;

public:
	static LogManager& Get();

	void LogMessage(LogSeverity Severity, const std::string& Category, const std::string& Message);

	template<typename... Args>
	void LogMessage(LogSeverity Severity, const std::string& Category, const std::string& Message, Args... args) {
		int length = 1 + snprintf(nullptr, 0, Message.c_str(), args...);
		char* buffer = new char[length];
		snprintf(buffer, length, Message.c_str(), args...);
		LogMessage(Severity, Category, buffer);
		delete[] buffer;
	}

	const std::unordered_set<std::string>& GetCategories();

private:
	std::vector<LogOutput*> Outputs;
	std::unordered_set<std::string> Categories;
};