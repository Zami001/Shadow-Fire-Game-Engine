#pragma once

#include <Core.h>
#include <Modules/Module.h>
#include <Modules/ModuleManager.h>
#include <concepts>
#include <iostream>

template<typename T, ModuleManager::LoadingPhase Phase = ModuleManager::LoadingPhase::Default>
requires(std::derived_from<T, Module>)
class ModuleRegister {
private:
	static_assert(Phase != ModuleManager::PreInit, "A module cannot be set to initialize during PreInit");

	static T& Get() {
		static T module;
		return module;
	}

public:
	ModuleRegister() {
		ModuleManager::Get().AddModule(&Get(), Phase);
	}

	~ModuleRegister() {
		if (Get().IsInitialized()) {
			ModuleManager::Get().RemoveModule(&Get());
		}
	}
};