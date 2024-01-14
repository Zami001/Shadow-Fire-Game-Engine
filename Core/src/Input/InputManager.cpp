#include <Input/InputManager.h>
#include <Windows.h>
#include <Xinput.h>

InputManager::InputManager() : MouseListeners(), ButtonListeners() {
	Buttons.resize(static_cast<int>(Keycode::MAX_VALUE), ButtonState::Up);
}

ButtonState InputManager::GetButtonState(Keycode code) {
	return Buttons[static_cast<int>(code)];
}

Vector2i InputManager::GetMousePosition() {
	return MousePosition;
}

InputManager::Handle InputManager::RegisterButtonListener(std::function<void(Keycode, ButtonState)> callback) {
	Handle handle = GetNewHandle();
	
	ButtonListeners[handle] = callback;

	return handle;
}

InputManager::Handle InputManager::RegisterMouseListener(std::function<void(Vector2i)> callback) {
	Handle handle = GetNewHandle();

	MouseListeners[handle] = callback;

	return handle;
}

void InputManager::UnregisterButtonListener(Handle handle) {
	auto listener = ButtonListeners.find(handle);
	if (listener != ButtonListeners.end()) {
		ButtonListeners.erase(listener);
	}
}

void InputManager::UnregisterMouseListener(Handle handle) {
	auto listener = MouseListeners.find(handle);
	if (listener != MouseListeners.end()) {
		MouseListeners.erase(listener);
	}
}

void InputManager::SetControllerVibration(int ControllerIndex, float Amount) {
	XINPUT_VIBRATION Vibration;
	unsigned short TrueAmount = max(Amount * 0xffff, 0xffff);
	Vibration.wLeftMotorSpeed = TrueAmount;
	Vibration.wRightMotorSpeed = TrueAmount;
	XInputSetState(ControllerIndex, &Vibration);
}

void InputManager::SetMousePosition(Vector2i Position) {
	MousePosition = Position;
	
	for (auto& listener : MouseListeners) {
		listener.second(Position);
	}
}

void InputManager::SetButtonDown(Keycode Key) {
	if (Buttons[static_cast<int>(Key)] == ButtonState::Down) return;

	Buttons[static_cast<int>(Key)] = ButtonState::Down;
	for (auto& listener : ButtonListeners) {
		listener.second(Key, ButtonState::Down);
	}
}

void InputManager::SetButtonUp(Keycode Key) {
	if (Buttons[static_cast<int>(Key)] == ButtonState::Up) return;

	Buttons[static_cast<int>(Key)] = ButtonState::Up;
	for (auto& listener : ButtonListeners) {
		listener.second(Key, ButtonState::Up);
	}
}

void InputManager::MouseWheelMoved(float Amount) {
	
}

void InputManager::HorizontalMouseWheelMoved(float Amount) {
	
}

void InputManager::UpdateControllerInput() {
	static_assert(XUSER_MAX_COUNT >= MaxControllers);

	XINPUT_STATE state;
	memset(&state, 0, sizeof(XINPUT_STATE));
	
	for (int i = 0; i < MaxControllers; ++i) {
		if (XInputGetState(i, &state) == 0) {
			// controller is connected
			Controllers[i].Connected = true;
			Controllers[i].ButtonStates = static_cast<Controller::Button>(state.Gamepad.wButtons);
			Controllers[i].RawLeftThumb = { state.Gamepad.sThumbLX, state.Gamepad.sThumbLY };
			Controllers[i].RawRightThumb = { state.Gamepad.sThumbRX, state.Gamepad.sThumbRY };
			Controllers[i].RawLeftTrigger = state.Gamepad.bLeftTrigger;
			Controllers[i].RawRightTrigger = state.Gamepad.bRightTrigger;

			if (std::abs(Controllers[i].RawLeftThumb.x) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && std::abs(Controllers[i].RawLeftThumb.y) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
				Controllers[i].RawLeftThumb = { 0, 0 };
			}

			if (std::abs(Controllers[i].RawRightThumb.x) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && std::abs(Controllers[i].RawRightThumb.y) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
				Controllers[i].RawRightThumb = { 0, 0 };
			}
		} else {
			// controller is disconnected
			Controllers[i].Connected = false;
		}
	}
}

InputManager::Handle InputManager::GetNewHandle() {
	static Handle nextHandle;
	return nextHandle++;
}