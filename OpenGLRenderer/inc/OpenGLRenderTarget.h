#pragma once

#include <OpenGLRenderer.h>
#include <RenderTarget.h>
#include <Math/Bounds2DInt.h>

class OpenGLWindow;

class OPENGL_API OpenGLRenderTarget : public RenderTarget {
public:
	OpenGLRenderTarget(Bounds2DInt size);
	~OpenGLRenderTarget();
	
	virtual void Bind() override;
	virtual void Clear() override;

private:
	uint32_t handle;
	uint32_t textureHandle;
};