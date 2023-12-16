#pragma once

#include <OpenGLRenderer.h>
#include <Window.h>
#include <OpenGLWindowRenderTarget.h>
#include <OpenGLInputManager.h>

class OPENGL_API OpenGLWindow : public Window {
public:
	OpenGLWindow(const WindowParams& params);
	~OpenGLWindow();

	virtual int GetWidth() override;
	virtual int GetHeight() override;
	virtual bool GetShouldClose() override;
	virtual void ProcessEvents() override;
	virtual void Present() override;
	virtual RenderTarget& GetRenderTarget() override;
	virtual InputManager& GetInputManager() override;

private:
	struct GLFWwindow* handle;
	OpenGLWindowRenderTarget renderTarget;
	OpenGLInputManager inputManager;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};