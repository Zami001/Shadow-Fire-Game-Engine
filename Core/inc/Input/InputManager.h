#pragma once

#include <Core.h>
#include <functional>
#include <vector>
#include <Input/Keycodes.h>

class CORE_API InputManager {
public:
	InputManager() = default;
	virtual ~InputManager() = default;

	virtual bool GetButtonDown(Keycode code) = 0;
};