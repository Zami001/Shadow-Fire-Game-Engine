#pragma once

#include <Core.h>
#include <vector>
#include <Modules/Module.h>
#include <map>

class CORE_API ModuleManager {
	friend Module;

public:
	enum LoadingPhase {
		PreInit = -1,
		Default
	};
	
	template<typename T, LoadingPhase Phase>
	requires(std::derived_from<T, Module>)
	friend class ModuleRegister;

public:
	static ModuleManager& Get();

	Module* GetModule(std::string ModuleName) const;

private:
	std::map<std::string, Module*> RegisteredModules;
	std::map<Module*, std::vector<Module*>> DependencyTree;
	std::map<LoadingPhase, std::vector<Module*>> AwaitingPhase;
	std::vector<Module*> AwaitingDependencies;

	LoadingPhase CurrentPhase = PreInit;

	void AddModule(Module* module, LoadingPhase phase);
	void RemoveModule(Module* module);
	
	void RegisterModule(Module* module);
	void UnregisterModule(Module* module);

	void InitializeModule(Module* module);
	void RecheckDepenencies();

public:
	const std::map<std::string, Module*>& GetModules() { return RegisteredModules; }
	LoadingPhase GetLoadingPhase() const { return CurrentPhase; }

	void ProgressLoadingPhase(LoadingPhase NewPhase);
	void ShutdownAllModules();
};