#include <Application.h>
#include <stdlib.h>
#include <csignal>

#include <vector>

#include <Profiling/ScopedProfiler.h>
#include <Profiling/TimedProfiler.h>
#include <MinimalCore.h>


#include <Logging/ConsoleLogger.h>
#include <stacktrace>
#include <DbgHelp.h>

Application* Application::instance;

void HandleSignal(int signum) {
	SF_LOG(LogStackTrace, Fatal, std::to_string(std::stacktrace::current()))
	
	exit(-1);
}

void Application::Init(int argc, char** argv) {
	signal(SIGSEGV, &HandleSignal);

	instance = new Application();

	instance->ExtractLaunchParameters(argc, argv);
}

void Application::Run(Game& game) {
	ConsoleLogger logger;
	instance->Running = true;
	instance->ActiveGame = &game;

	game.Initialize();
	game.CreateInitialScene();

	{
		WindowParams winParams;
		winParams.VSync = false;
		winParams.size = {2560, 1440};
		winParams.style = WindowStyle::Borderless;
		//instance->Windows.push_back(new Window(winParams));
		instance->Windows.push_back(game.GetRenderer().CreateWindow(winParams));

		while (instance->Running) {
			game.Step();

			for (int i = 0; i < instance->Windows.size(); ++i) {
				instance->Windows[i]->Present();
				instance->Windows[i]->ProcessEvents();

				// close windows which need to be terminated and remove them from the list
				if (instance->Windows[i]->GetShouldClose()) {
					instance->Windows[i] = instance->Windows[instance->Windows.size() - 1];

					instance->Windows.pop_back();
					--i;
				}
			}

			if (instance->Windows.size() == 0) {
				instance->Running = false;
			}
		}
	}

	game.Shutdown();
}

void Application::Shutdown() {
	instance->Running = false;
}

void Application::Terminate() {
	delete instance;
	exit(0);
}

bool Application::ContainsLaunchOption(const char* Option, char**& OutParameters, int& OutParameterCount) {
	for (int i = 0; i < instance->LaunchParameters.size(); ++i) {
		if (strcmp(Option, instance->LaunchParameters[i]) == 0) {
			for (int j = i + 1; j < instance->LaunchParameters.size(); ++j) {
				if (instance->LaunchParameters[j][0] != '/') {
					OutParameters = &instance->LaunchParameters.data()[i + 1];
					OutParameterCount = j - i - 1;
					return true;
				}
			}

			if (instance->LaunchParameters.size() == i) {
				OutParameterCount = 0;
			} else {
				OutParameters = &instance->LaunchParameters.data()[i + 1];
				OutParameterCount = instance->LaunchParameters.size() - i;
			}

			return true;
		}
	}

	return false;
}

const std::vector<char*>& Application::GetLaunchOptions() {
	return instance->LaunchParameters;
}

void Application::ExtractLaunchParameters(int argc, char** argv) {
	LaunchParameters.reserve(argc - 1);

	// first argument is always program name so write it to ApplicationPath then skip
	int offset = std::string(argv[0]).find_last_of('\\');
	argv[0][offset] = '\0';

	ApplicationPath = argv[0];

	for (int i = 1; i < argc; ++i) {
		LaunchParameters.push_back(argv[i]);
	}
}

const std::vector<SFSharedPtr<Window>>& Application::GetWindows() {
	return instance->Windows;
}

std::filesystem::path Application::GetApplicationPath() {
	return instance->ApplicationPath;
}