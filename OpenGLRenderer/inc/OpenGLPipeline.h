#pragma once

#include <OpenGLRenderer.h>
#include <RenderPipeline.h>
#include <Rendering.h>

class OPENGL_API OpenGLPipeline : public RenderPipeline {
public:
	static OpenGLPipeline* CreatePipeline();

	virtual void Init() override;
	virtual void Shutdown() override;

	virtual Window* CreateWindow(const WindowParams& params) override;
	virtual Mesh* CreateMesh() override;
	virtual Material* GetDefaultMaterial() const override;
	virtual RenderTarget* CreateRenderTarget(const Bounds2DInt& bounds) override;
	virtual void BeginRender(const Camera& camera) override;
	virtual void EndRender(const Camera& camera) override;
	virtual Texture2D* CreateTexture2D() override { throw std::exception("Not implemented"); }

private:
	static void Error_Callback(int error, const char* description);
};