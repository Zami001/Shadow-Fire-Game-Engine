#include <OpenGLRenderTarget.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

OpenGLRenderTarget::OpenGLRenderTarget(Bounds2DInt size) : RenderTarget() {
	glGenFramebuffers(1, &handle);
	glBindFramebuffer(GL_FRAMEBUFFER, handle);

	glGenTextures(1, &textureHandle);

	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
}

OpenGLRenderTarget::~OpenGLRenderTarget() {
	if (handle) {
		glDeleteFramebuffers(1, &handle);
	}
}

void OpenGLRenderTarget::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, handle);

	Bounds2DInt viewport = GetViewport();
	glViewport(viewport.position.x, viewport.position.y, viewport.size.x, viewport.size.y);
}

void OpenGLRenderTarget::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}