#include <DX11Material.h>
#include <DX11Shader.h>
#include <DX11Pipeline.h>

typedef struct _constantBufferStruct {
	Matrix4x4 mvp;
} ConstantBufferStruct;

DX11Material::~DX11Material() {}

bool DX11Material::SetupMaterial() {
	// Create mvp matrix buffer
	CD3D11_BUFFER_DESC cbDesc(
		sizeof(ConstantBufferStruct),
		D3D11_BIND_CONSTANT_BUFFER
	);

	DX11Pipeline::device->CreateBuffer(
		&cbDesc,
		nullptr,
		ConstantBuffer.GetAddressOf()
	);

	return true;
}