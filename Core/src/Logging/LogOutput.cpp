#include <Logging/LogOutput.h>
#include <Logging/LogManager.h>

LogOutput::LogOutput() {
	LogManager::Get().Outputs.push_back(this);
}

LogOutput::~LogOutput() {
	for (auto iter = LogManager::Get().Outputs.begin(); iter != LogManager::Get().Outputs.end(); iter++) {
		if (*iter == this) {
			LogManager::Get().Outputs.erase(iter);
			break;
		}
	}
}