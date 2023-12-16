#pragma once

#include <DX11Renderer.h>
#include <Material.h>
#include <DX11Pipeline.h>

class DX11Shader;

class DX11_API DX11Material : public Material {
	friend class DX11Mesh;

public:
	DX11Material() = default;
	~DX11Material();

protected:
	virtual bool SetupMaterial() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
};