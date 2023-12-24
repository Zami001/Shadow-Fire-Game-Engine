#pragma once

#include <DX11Renderer.h>
#include <RenderPipeline.h>
#include <MinimalCore.h>
#include <ShaderIncluder.h>
#include <wrl.h>
#include <dxgi1_3.h>
#include <d3d11_3.h>

#include <combaseapi.h>
#include <wincodec.h>

class DX11Material;

class DX11_API DX11Pipeline : public RenderPipeline {
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual SFSharedPtr<Window> CreateWindow(const WindowParams& params) override;
	virtual SFSharedRef<Mesh> CreateMesh() override;
	virtual SFSharedRef<Material> CreateMaterial() override;
	virtual SFSharedRef<Material> GetDefaultMaterial() const override;
	virtual SFSharedRef<Material> GetErrorMaterial() const override;
	virtual SFSharedRef<RenderTarget> CreateRenderTarget(const Bounds2Di& bounds) override;
	virtual void BeginRender(const Camera& camera) override;
	virtual void EndRender(const Camera& camera) override;
	virtual SFSharedRef<Texture2D> CreateTexture2D() override;

	static Microsoft::WRL::ComPtr<ID3D11Device> device;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	static Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer;
	static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	static Microsoft::WRL::ComPtr<IWICImagingFactory> ImageFactory;
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;

	static ShaderIncluder Includer;

protected:
	virtual SFSharedRef<Shader> CreateShader(ShaderType type) override;

private:
	SFSharedPtr<Material> DefaultMaterial;
	SFSharedPtr<Material> ErrorMaterial;

	void SetupDefaultMaterial();
	void SetupErrorMaterial();
};