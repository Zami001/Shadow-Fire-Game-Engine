#include <Modules/CoreModule.h>
#include <Modules/ModuleRegister.h>

ModuleRegister<CoreModule> coreModule;

const std::string& CoreModule::GetModuleName() const {
	static std::string name = "CoreModule";
	return name;
}
