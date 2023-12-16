#include <OpenGLWindowRenderTarget.h>

#include <OpenGLWindow.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

OpenGLWindowRenderTarget::OpenGLWindowRenderTarget(OpenGLWindow* window) : RenderTarget(), window(window) {
	
}

OpenGLWindowRenderTarget::~OpenGLWindowRenderTarget() {

}

void OpenGLWindowRenderTarget::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	BindViewport();
}

void OpenGLWindowRenderTarget::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindowRenderTarget::SetViewport(Bounds2DInt Viewport) {
	RenderTarget::SetViewport(Viewport);

	BindViewport();
}

void OpenGLWindowRenderTarget::BindViewport() {
	Bounds2DInt viewport = GetViewport();
	glViewport(viewport.position.x, viewport.position.y, viewport.size.x, viewport.size.y);
}