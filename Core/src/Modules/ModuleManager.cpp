#include <Modules/ModuleManager.h>
#include <Asserts.h>

ModuleManager& ModuleManager::Get() {
	static ModuleManager Manager;
	return Manager;
}

Module* ModuleManager::GetModule(std::string ModuleName) const {
	auto found = RegisteredModules.find(ModuleName);
	if (found != RegisteredModules.end()) {
		return found->second;
	}

	return nullptr;
}

void ModuleManager::AddModule(Module* module, LoadingPhase phase) {
	if (static_cast<int>(phase) > CurrentPhase) {
		AwaitingPhase[phase].push_back(module);
		return;
	}

	RegisterModule(module);
}

void ModuleManager::RemoveModule(Module* module) {
	UnregisterModule(module);
}

void ModuleManager::RegisterModule(Module* module) {
	// todo: should probably be looking for circular dependency chains in some way

	if (const std::vector<std::string>* Dependencies = module->GetDependencies()) {
		for (int i = 0; i < Dependencies->size(); ++i) {
			if (RegisteredModules.count(Dependencies->at(i)) == 0) {
				AwaitingDependencies.push_back(module);
				return;
			}
		}
	}

	InitializeModule(module);
	RecheckDepenencies();
}

void ModuleManager::UnregisterModule(Module* module) {
	// Unregister all modules that are dependant on this module
	auto Dependants = DependencyTree.find(module);
	if (Dependants != DependencyTree.end()) {
		while (Dependants->second.size() > 0) {
			UnregisterModule(Dependants->second[0]);
		}

		DependencyTree.erase(module);
	}

	std::string moduleName = module->GetModuleName();
	SF_LOG(Module, Log, "Uninitializing %s...", moduleName.c_str());

	RegisteredModules.erase(moduleName);

	module->UninitializeModule();
	module->isInitialized = false;

	// remove the module from dependency tree
	if (module->GetDependencies()) {
		for (int i = 0; i < module->GetDependencies()->size(); ++i) {
			Dependants = DependencyTree.find(GetModule(module->GetDependencies()->at(i)));
			for (int y = 0; y < Dependants->second.size(); ++y) {
				if (Dependants->second[y] == module) {
					Dependants->second[y] = Dependants->second[Dependants->second.size() - 1];
					Dependants->second.pop_back();
					break;
				}
			}
		}
	}
}

void ModuleManager::InitializeModule(Module* module) {
	std::string moduleName = module->GetModuleName();

	SF_ASSERT(RegisteredModules.count(module->GetModuleName()) == 0, "Error initializing module %s, a module with that name already exists", moduleName.c_str());
	SF_LOG(Module, Log, "Initializing %s...", moduleName.c_str());

	module->InitializeModule();
	module->isInitialized = true;

	RegisteredModules.emplace(module->GetModuleName(), module);

	// add to dependency tree
	if (module->GetDependencies() != nullptr) {
		for (int i = 0; i < module->GetDependencies()->size(); ++i) {
			Module* dependant = GetModule(module->GetDependencies()->at(i));

			DependencyTree[dependant].push_back(module);
		}
	}
}

void ModuleManager::RecheckDepenencies() {
	for (int i = 0; i < AwaitingDependencies.size(); ++i) {
		bool DependenciesFound = true;
		
		for (int y = 0; y < AwaitingDependencies[i]->GetDependencies()->size(); ++y) {
			if (RegisteredModules.count(AwaitingDependencies[i]->GetDependencies()->at(y)) == 0) {
				DependenciesFound = false;
				break;
			}
		}

		if (DependenciesFound) {
			InitializeModule(AwaitingDependencies[i]);
			AwaitingDependencies[i] = AwaitingDependencies[AwaitingDependencies.size() - 1];
			AwaitingDependencies.pop_back();
			--i;
		}
	}
}

void ModuleManager::ProgressLoadingPhase(LoadingPhase NewPhase) {
	if (static_cast<int>(NewPhase) <= static_cast<int>(CurrentPhase)) {
		SF_LOG(Module, Warning, "Current module initialization phase cannot be progressed from %i to %i", static_cast<int>(CurrentPhase), static_cast<int>(NewPhase));
		return;
	}

	do {
		CurrentPhase = static_cast<LoadingPhase>(static_cast<int>(CurrentPhase) + 1);
		auto PhaseInit = AwaitingPhase.find(CurrentPhase);
		if (PhaseInit == AwaitingPhase.end()) {
			continue;
		}

		for (int i = 0; i < PhaseInit->second.size(); ++i) {
			RegisterModule(PhaseInit->second[i]);
		}

		AwaitingPhase.erase(CurrentPhase);
	} while (CurrentPhase < NewPhase);
}

void ModuleManager::ShutdownAllModules() {
	while (RegisteredModules.size() > 0) {
		UnregisterModule(RegisteredModules.begin()->second);
	}
}
