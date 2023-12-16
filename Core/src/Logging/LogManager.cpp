#include <Logging/LogManager.h>
#include <stdio.h>

LogManager& LogManager::Get() {
	static LogManager Manager;
	return Manager;
}

void LogManager::LogMessage(LogSeverity Severity, const std::string& Category, const std::string& Message) {
	Categories.emplace(Category);

	for (auto logger : Outputs) {
		logger->OutputLog(Severity, Category, Message);
	}
}

const std::unordered_set<std::string>& LogManager::GetCategories() {
	return Categories;
}