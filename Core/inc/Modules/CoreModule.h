#pragma once

#include <Core.h>
#include <Modules/Module.h>

class CORE_API CoreModule : public Module {
public:
	CoreModule() = default;
	~CoreModule() = default;

	virtual const std::string& GetModuleName() const override;
};