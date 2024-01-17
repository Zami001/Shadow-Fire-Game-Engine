#pragma once

#include <Core.h>
#include <functional>
#include <vector>
#include <Input/Keycodes.h>
#include <MinimalCore.h>
#include <Input/Gamepad.h>

class CORE_API InputManager {
public:
	InputManager();
	virtual ~InputManager() = default;

	ButtonState GetButtonState(Keycode code);
	Vector2i GetMousePosition();

	Gamepad& GetGamepad(int index);

	MulticastDelegate<Keycode, ButtonState> OnButtonEvent;
	MulticastDelegate<Vector2i> OnMouseMoved;
	MulticastDelegate<Vector2> OnMouseScroll;

protected:
	void SetMousePosition(Vector2i Position);
	void SetButtonState(Keycode Key, ButtonState State);
	void MouseWheelMoved(float Amount);
	void HorizontalMouseWheelMoved(float Amount);
	void UpdateControllerInput();

private:
	Vector2i MousePosition;
	ButtonState KeyboardButtons[static_cast<int>(Keycode::MAX_VALUE)];
	
	constexpr static int MaxControllers = 4;
	Gamepad Gamepads[MaxControllers];
};