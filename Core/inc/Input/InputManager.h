#pragma once

#include <Core.h>
#include <functional>
#include <vector>
#include <Input/Keycodes.h>
#include <MinimalCore.h>
#include <map>
#include <Input/Controller.h>

class CORE_API InputManager {
public:
	typedef size_t Handle;

	InputManager();
	virtual ~InputManager() = default;

	ButtonState GetButtonState(Keycode code);
	Vector2i GetMousePosition();

	Handle RegisterButtonListener(std::function<void(Keycode, ButtonState)> callback);
	Handle RegisterMouseListener(std::function<void(Vector2i)> callback);
	
	void UnregisterButtonListener(Handle handle);
	void UnregisterMouseListener(Handle handle);

	void SetControllerVibration(int ControllerIndex, float Amount);

protected:
	void SetMousePosition(Vector2i Position);
	void SetButtonDown(Keycode Key);
	void SetButtonUp(Keycode Key);
	void MouseWheelMoved(float Amount);
	void HorizontalMouseWheelMoved(float Amount);
	void UpdateControllerInput();

	static Handle GetNewHandle();

private:
	std::map<Handle, std::function<void(Keycode, ButtonState)>> ButtonListeners;
	std::map<Handle, std::function<void(Vector2i)>> MouseListeners;

	Vector2i MousePosition;
	std::vector<ButtonState> Buttons;
	
	constexpr static int MaxControllers = 4;
	Controller Controllers[MaxControllers];
};