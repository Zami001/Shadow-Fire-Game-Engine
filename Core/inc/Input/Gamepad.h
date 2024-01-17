#pragma once

#include <Core.h>
#include <Math/Vector.h>
#include <map>
#include <functional>
#include <Input/Keycodes.h>
#include <MulticastDelegate.h>

class CORE_API Gamepad {
public:
	friend class InputManager;

	typedef uint64_t Handle;

	enum class Button {
		None = 0,
		DPadUp = 1 << 0,
		DPadDown = 1 << 1,
		DPadLeft = 1 << 2,
		DPadRight = 1 << 3,
		Start = 1 << 4,
		Back = 1 << 5,
		LeftThumb = 1 << 6,
		RightThumb = 1 << 7,
		LeftShoulder = 1 << 8,
		RightShoulder = 1 << 9,
		A = 1 << 12,
		B = 1 << 13,
		X = 1 << 14,
		Y = 1 << 15,

		MAX_VALUE
	};

private:
	bool Connected = false;
	Button ButtonStates = Button::None;
	uint8_t RawLeftTrigger;
	uint8_t RawRightTrigger;
	Vector<int16_t, 2> RawLeftThumb;
	Vector<int16_t, 2> RawRightThumb;
	unsigned char GamepadIndex;
	std::map<Handle, std::function<void(bool)>> ConnectionListeners;
	std::map<Handle, std::function<void(Button, ButtonState)>> ButtonListeners;
	std::map<Handle, std::function<void(int, float)>> TriggerListeners;
	std::map<Handle, std::function<void(int, Vector2)>> JoystickListeners;

	void UpdateInputs();
	static Handle GetNewHandle();

public:
	inline int GetGamepadIndex() const { return GamepadIndex; }

	inline bool IsConnected() const { return Connected; }

	inline bool GetButtonPressed(Button button) const {
		return static_cast<int>(ButtonStates) & static_cast<int>(button);
	}

	inline float GetLeftTrigger() const {
		return RawLeftTrigger / static_cast<float>(0xff);
	}

	inline float GetRightTrigger() const {
		return RawRightTrigger / static_cast<float>(0xff);
	}

	inline Vector2 GetLeftThumbStick() const {
		return Vector2(RawLeftThumb.x / static_cast<float>(0x8000), RawLeftThumb.y / static_cast<float>(0x8000));
	}

	inline Vector2 GetRightThumbStick() const {
		return Vector2(RawRightThumb.x / static_cast<float>(0x8000), RawRightThumb.y / static_cast<float>(0x8000));
	}

	void SetVibration(float Amount);

	Handle RegisterConnectionListener(std::function<void(bool)> callback);
	Handle RegisterButtonListener(std::function<void(Button, ButtonState)> callback);
	Handle RegisterTriggerListener(std::function<void(int, float)> callback);
	Handle RegisterJoystickListener(std::function<void(int, Vector2)> callback);

	void UnregisterConnectionListener(Handle handle);
	void UnregisterButtonListener(Handle handle);
	void UnregisterTriggerListener(Handle handle);
	void UnregisterJoystickListener(Handle handle);

	MulticastDelegate<bool> OnConnectionEvent;
	MulticastDelegate<Button, ButtonState> OnButtonEvent;
	MulticastDelegate<int, float> OnTriggerEvent;
	MulticastDelegate<int, Vector2> OnJoystickEvent;
};