#include <Input/InputManager.h>
#include <Windows.h>
#include <Xinput.h>

InputManager::InputManager() {
	memset(&KeyboardButtons, static_cast<int>(ButtonState::Up), sizeof(ButtonState) * static_cast<int>(Keycode::MAX_VALUE));

	for (int i = 0; i < MaxControllers; ++i) {
		Gamepads[i].GamepadIndex = i;
	}
}

ButtonState InputManager::GetButtonState(Keycode code) {
	return KeyboardButtons[static_cast<int>(code)];
}

Vector2i InputManager::GetMousePosition() {
	return MousePosition;
}

Gamepad& InputManager::GetGamepad(int index) {
	return Gamepads[index];
}

void InputManager::SetMousePosition(Vector2i Position) {
	MousePosition = Position;
	OnMouseMoved(MousePosition);
}

void InputManager::SetButtonState(Keycode Key, ButtonState State) {
	if (KeyboardButtons[static_cast<int>(Key)] == State) return;

	KeyboardButtons[static_cast<int>(Key)] = State;
	OnButtonEvent(Key, State);
}

void InputManager::MouseWheelMoved(float Amount) {
	OnMouseScroll({ 0.f, Amount });
}

void InputManager::HorizontalMouseWheelMoved(float Amount) {
	OnMouseScroll({ Amount, 0.f });
}

void InputManager::UpdateControllerInput() {
	static_assert(XUSER_MAX_COUNT >= MaxControllers);
	
	for (int i = 0; i < MaxControllers; ++i) {
		Gamepads[i].UpdateInputs();
	}
}