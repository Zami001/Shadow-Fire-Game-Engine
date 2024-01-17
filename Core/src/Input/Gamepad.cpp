#include <Input/Gamepad.h>
#include <Windows.h>
#include <Xinput.h>

#include <MinimalCore.h>

void Gamepad::UpdateInputs() {
	XINPUT_STATE state;

	if (XInputGetState(GamepadIndex, &state) == 0) {
		// controller is connected
		if (!Connected) {
			
		}
		Gamepad::Button NewButtonStates = static_cast<Gamepad::Button>(state.Gamepad.wButtons);


		Connected = true;
		ButtonStates = NewButtonStates;
		RawLeftThumb = { state.Gamepad.sThumbLX, state.Gamepad.sThumbLY };
		RawRightThumb = { state.Gamepad.sThumbRX, state.Gamepad.sThumbRY };
		RawLeftTrigger = state.Gamepad.bLeftTrigger;
		RawRightTrigger = state.Gamepad.bRightTrigger;

		if (std::abs(RawLeftThumb.x) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && std::abs(RawLeftThumb.y) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			RawLeftThumb = { 0, 0 };
		}

		if (std::abs(RawRightThumb.x) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && std::abs(RawRightThumb.y) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
			RawRightThumb = { 0, 0 };
		}
	} else {
		// controller is disconnected
		Connected = false;
	}
}

Gamepad::Handle Gamepad::GetNewHandle() {
	static Handle NextHandle = 0;
	return NextHandle++;
}

void Gamepad::SetVibration(float Amount) {
	XINPUT_VIBRATION Vibration;
	unsigned short TrueAmount = min(Amount * 0xffff, 0xffff);
	Vibration.wLeftMotorSpeed = TrueAmount;
	Vibration.wRightMotorSpeed = TrueAmount;
	XInputSetState(GamepadIndex, &Vibration);
}

Gamepad::Handle Gamepad::RegisterConnectionListener(std::function<void(bool)> callback) {
	Handle handle = GetNewHandle();
	ConnectionListeners[handle] = callback;
	return handle;
}

Gamepad::Handle Gamepad::RegisterButtonListener(std::function<void(Button, ButtonState)> callback) {
	Handle handle = GetNewHandle();
	ButtonListeners[handle] = callback;
	return handle;
}

Gamepad::Handle Gamepad::RegisterTriggerListener(std::function<void(int, float)> callback) {
	Handle handle = GetNewHandle();
	TriggerListeners[handle] = callback;
	return handle;
}

Gamepad::Handle Gamepad::RegisterJoystickListener(std::function<void(int, Vector2)> callback) {
	Handle handle = GetNewHandle();
	JoystickListeners[handle] = callback;
	return handle;
}

void Gamepad::UnregisterConnectionListener(Handle handle) {
	auto listener = ConnectionListeners.find(handle);
	if (listener != ConnectionListeners.end()) {
		ConnectionListeners.erase(listener);
	}
}

void Gamepad::UnregisterButtonListener(Handle handle) {
	auto listener = ButtonListeners.find(handle);
	if (listener != ButtonListeners.end()) {
		ButtonListeners.erase(listener);
	}
}

void Gamepad::UnregisterTriggerListener(Handle handle) {
	auto listener = TriggerListeners.find(handle);
	if (listener != TriggerListeners.end()) {
		TriggerListeners.erase(listener);
	}
}

void Gamepad::UnregisterJoystickListener(Handle handle) {
	auto listener = JoystickListeners.find(handle);
	if (listener != JoystickListeners.end()) {
		JoystickListeners.erase(listener);
	}
}
