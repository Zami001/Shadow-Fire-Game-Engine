#pragma once

#include <DX11Renderer.h>
#include <Window.h>
#include <wrl.h>
#include <dxgi1_3.h>
#include <d3d11_3.h>

class DX11_API DX11Window : public Window {
	friend class DX11Pipeline;

public:
	DX11Window(const WindowParams& params);
	
	virtual void Present() override;

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
	D3D11_VIEWPORT Viewport;
};