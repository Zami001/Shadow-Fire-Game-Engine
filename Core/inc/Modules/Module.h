#pragma once

#include <Core.h>
#include <string>
#include <vector>

class CORE_API Module {
	friend class ModuleManager;

public:
	Module();
	virtual ~Module();

	virtual const std::string& GetModuleName() const = 0;
	virtual const std::vector<std::string>* GetDependencies() const { return nullptr; }

protected:
	virtual void InitializeModule() {}
	virtual void UninitializeModule() {}

private:
	bool isInitialized = false;

public:
	bool IsInitialized() const { return isInitialized; }
};