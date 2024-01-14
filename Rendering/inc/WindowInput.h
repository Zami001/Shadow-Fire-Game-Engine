#pragma once

#include <Rendering.h>
#include <Input/InputManager.h>
#include <MinimalCore.h>

class RENDERING_API WindowInput : public InputManager {
	friend class Window;

public:
	WindowInput();
	~WindowInput();
};