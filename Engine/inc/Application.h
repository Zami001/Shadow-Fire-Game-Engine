#pragma once

#include <Engine.h>
#include <vector>
#include <Game.h>
#include <filesystem>
#include <MinimalCore.h>

class ENGINE_API Application {
private:
	Application() = default;
	~Application() = default;

	static Application* instance;

public:
	static void Init(int argc, char** argv);

	static void Run(Game& game);

	/// @brief Called to shut down the entire application cleanly
	static void Shutdown();

	/// @brief Called when all threads are closed and the engine is ready to terminate
	static void Terminate();

	static bool ContainsLaunchOption(const char* Option, char**& OutParameters, int& OutParameterCount);
	static const std::vector<char*>& GetLaunchOptions();

	static const std::vector<SFSharedPtr<Window>>& GetWindows();

	static std::filesystem::path GetApplicationPath();

	static Game& GetActiveGame() { return *instance->ActiveGame; }

private:
	bool Running = true;
	std::vector<char*> LaunchParameters;
	std::vector<SFSharedPtr<Window>> Windows;

	std::filesystem::path ApplicationPath;

	Game* ActiveGame = nullptr;

	void ExtractLaunchParameters(int argc, char** argv);
};