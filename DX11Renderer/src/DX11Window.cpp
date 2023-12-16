#include <DX11Window.h>
#include <DX11Pipeline.h>
#include <DirectXMath.h>

DX11Window::DX11Window(const WindowParams& params) : Window(params) {

}

void DX11Window::Present() {
	if (VSync) {
		SwapChain->Present(1, 0);
	} else {
		SwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
	}

	const float black[] = {.0f, .0f, .0f, 1.f};
	DX11Pipeline::context->ClearRenderTargetView(RenderTarget.Get(), black);
	DX11Pipeline::context->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	DX11Pipeline::context->OMSetRenderTargets(1, RenderTarget.GetAddressOf(), DepthStencilView.Get());
}