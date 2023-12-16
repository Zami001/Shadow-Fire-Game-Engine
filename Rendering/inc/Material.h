#pragma once

#include <Rendering.h>
#include <Shader.h>
#include <Texture/Texture2D.h>
#include <MinimalCore.h>

enum class RENDERING_API RenderPass : int {
	None,
	Lighting,
	Geometry,
	PostProcessing,
	GUI
};

class RENDERING_API Material {
protected:
	Material() : vertex(nullptr), fragment(nullptr), pass(RenderPass::None) {}

public:
	virtual ~Material() = default;

	RenderPass pass;

	std::vector<SFSharedRef<Texture2D>> Textures;

	bool BindShaders(const std::vector<SFSharedRef<Shader>>& Shaders);

protected:
	virtual bool SetupMaterial() = 0;

	SFSharedPtr<Shader> vertex;
	SFSharedPtr<Shader> fragment;

private:
	bool isBound = false;

public:
	inline bool GetIsBound() const {
		return isBound;
	}
};