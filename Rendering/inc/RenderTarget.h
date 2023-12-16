#pragma once

#include <Rendering.h>
#include <stdint.h>
#include <Math/Vector.h>
#include <MinimalCore.h>

class RENDERING_API RenderTarget {
protected:
	RenderTarget();

public:
	virtual ~RenderTarget();

	inline uint32_t GetWidth() { return width; }
	inline uint32_t GetHeight() { return height; }

	virtual void SetWidth(uint32_t NewWidth) { width = NewWidth; }
	virtual void SetHeight(uint32_t NewHeight) { height = NewHeight; }

	inline Bounds2Di GetViewport() { return viewport; }
	virtual void SetViewport(Bounds2Di Viewport) { viewport = Viewport; }

	virtual void Bind() = 0;
	virtual void Clear() = 0;

private:
	uint32_t width;
	uint32_t height;
	Bounds2Di viewport;
};