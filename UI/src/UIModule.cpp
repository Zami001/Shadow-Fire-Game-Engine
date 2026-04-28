#include <UIModule.h>
#include <Modules/ModuleRegister.h>
#include <Font.h>

ModuleRegister<UIModule> uiModule;

const std::string& UIModule::GetModuleName() const {
	static std::string name = "UIModule";
	return name;
}

void UIModule::InitializeModule() {
	Font::InitializeFreeType();
}

void UIModule::UninitializeModule() {
	Font::DeinitializeFreeType();
}