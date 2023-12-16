#include <OpenGLWindow.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

OpenGLWindow::OpenGLWindow(const WindowParams& params) : Window(params), renderTarget(this), inputManager() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwWindowHint(GLFW_DECORATED, params.style != WindowStyle::Borderless);

	handle = glfwCreateWindow(params.size.x, params.size.y, params.title, params.style == WindowStyle::Fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	glfwMakeContextCurrent(handle);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(params.VSync);

	glfwSetKeyCallback(handle, key_callback);
	inputManager.windowHandle = handle;
}

OpenGLWindow::~OpenGLWindow() {
	glfwDestroyWindow(handle);
}

int OpenGLWindow::GetWidth() {
	int width, height;
	
	return width;
}

int OpenGLWindow::GetHeight() {
	int width, height;
	
	return height;
}

//void OpenGLWindow::GetSize(int& width, int& height) {
//	glfwGetFramebufferSize(handle, &width, &height);
//}

bool OpenGLWindow::GetShouldClose() {
	return glfwWindowShouldClose(handle);
}

void OpenGLWindow::Present() {
	glfwSwapBuffers(handle);
}

void OpenGLWindow::ProcessEvents() {
	glfwPollEvents();

	int width, height;
	//GetSize(width, height);
	//renderTarget.SetViewport({ 0, 0, width, height });
}

RenderTarget& OpenGLWindow::GetRenderTarget() {
	return renderTarget;
}

InputManager& OpenGLWindow::GetInputManager() {
	return inputManager;
}

void OpenGLWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}