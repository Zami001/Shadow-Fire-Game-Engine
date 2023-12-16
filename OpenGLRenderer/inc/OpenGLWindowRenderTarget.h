#pragma once

#include <OpenGLRenderer.h>
#include <RenderTarget.h>

class OpenGLWindow;

class OPENGL_API OpenGLWindowRenderTarget : public RenderTarget {
public:
	OpenGLWindowRenderTarget(OpenGLWindow* window);
	~OpenGLWindowRenderTarget();

	virtual void SetWidth(uint32_t NewWidth) override {}
	virtual void SetHeight(uint32_t NewHeight) override {}

	virtual void Bind() override;
	virtual void Clear() override;

	virtual void SetViewport(Bounds2DInt Viewport) override;

private:
	OpenGLWindow* window;

	void BindViewport();
};
