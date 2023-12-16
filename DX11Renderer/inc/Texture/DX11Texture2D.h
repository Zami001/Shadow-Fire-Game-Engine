#pragma once

#include <DX11Renderer.h>
#include <Texture/Texture2D.h>

#include <combaseapi.h>
#include <wincodec.h>
#include <dxgi1_3.h>
#include <d3d11_3.h>
#include <wrl.h>

class DX11_API DX11Texture2D : public Texture2D {
public:
	DX11Texture2D() = default;
	virtual ~DX11Texture2D() = default;

	virtual void LoadFile(std::filesystem::path FilePath) override;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;
};