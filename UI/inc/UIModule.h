#pragma once

#include <UI.h>
#include <Modules/Module.h>

class UI_API UIModule : public Module {
public:
	UIModule() = default;
	~UIModule() = default;

	virtual const std::string& GetModuleName() const override;

protected:
	virtual void InitializeModule() override;
	virtual void UninitializeModule() override;
};