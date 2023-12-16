#pragma once

#include <OpenGLRenderer.h>
#include <Input/InputManager.h>

struct GLFWwindow;

class OPENGL_API OpenGLInputManager : public InputManager {
	friend class OpenGLWindow;

public:
	OpenGLInputManager() = default;
	~OpenGLInputManager() = default;

	virtual bool GetButtonDown(Keycode code) override;

private:
	GLFWwindow* windowHandle;

	constexpr int KeycodeToGlfw(Keycode key);
};