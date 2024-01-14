#include <Logging/ConsoleLogger.h>
#include <iostream>

void ConsoleLogger::OutputLog(LogSeverity Severity, const std::string& Category, const std::string& Message) {
	switch (Severity) {
        case LogSeverity::VeryVerbose:
            std::cout << "\x1B[90mVeryVerbose: " << Message << "\033[0m\n";
            break;

        case LogSeverity::Verbose:
            std::cout << "\x1B[90mVerbose: " << Message << "\033[0m\n";
            break;

        case LogSeverity::Profiling:
            std::cout << "\x1B[37mProfiling: " << Message << "\033[0m\n";
            break;

        case LogSeverity::Log:
            std::cout << "\x1B[0mLog: " << Message << "\n";
            break;

        case LogSeverity::Warning:
            std::cout << "\x1B[33mWarning: " << Message << "\033[0m\n";
            break;

        case LogSeverity::Error:
            std::cout << "\x1B[31mError: " << Message << "\033[0m\n";
            break;

        case LogSeverity::Fatal:
            std::cout << "\x1B[31mFatal: " << Message << "\033[0m\n";
            break;

        default:
            std::cout << "\x1B[0mUnknown Severity: " << Message << "\n";
            break;
	}
}