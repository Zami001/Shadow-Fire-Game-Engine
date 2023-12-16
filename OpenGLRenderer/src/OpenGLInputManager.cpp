#include <OpenGLInputManager.h>
#include <GLFW/glfw3.h>

bool OpenGLInputManager::GetButtonDown(Keycode code) {
	return glfwGetKey(windowHandle, KeycodeToGlfw(code));
}

constexpr int OpenGLInputManager::KeycodeToGlfw(Keycode key) {
	switch(key) {
		case Keycode::LeftArrow: return GLFW_KEY_LEFT;
		case Keycode::UpArrow: return GLFW_KEY_UP;
		case Keycode::RightArrow: return GLFW_KEY_RIGHT;
		case Keycode::DownArrow: return GLFW_KEY_DOWN;
	
		default:
			return -1;
	}
}