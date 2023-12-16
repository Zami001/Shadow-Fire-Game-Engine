#pragma once

#include <Rendering.h>
#include <Window.h>
#include <Mesh.h>
#include <RenderTarget.h>
#include <Camera.h>
#include <filesystem>
#include <Texture/Texture2D.h>
#include <MinimalCore.h>

class RENDERING_API RenderPipeline {
protected:
	RenderPipeline() = default;

public:
	virtual ~RenderPipeline() = default;

	virtual void Init();
	virtual void Shutdown();

	virtual SFSharedPtr<Window> CreateWindow(const WindowParams& params) = 0;
	virtual SFSharedRef<Mesh> CreateMesh() = 0;
	virtual SFSharedRef<Material> GetDefaultMaterial() const = 0;
	virtual SFSharedRef<Material> GetErrorMaterial() const = 0;
	virtual SFSharedRef<RenderTarget> CreateRenderTarget(const Bounds2Di& bounds) = 0;
	virtual void BeginRender(const Camera& camera) = 0;
	virtual void EndRender(const Camera& camera) = 0;
	virtual SFSharedRef<Shader> CreateShader(ShaderType type) = 0;
	virtual SFSharedRef<Texture2D> CreateTexture2D() = 0;
};