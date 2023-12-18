#include <DX11Pipeline.h>
#include <DX11Window.h>
#include <DX11Shader.h>
#include <DX11Material.h>
#include <Math/Vector.h>
#include <DX11Mesh.h>
#include <Texture/DX11Texture2D.h>
#include <ShaderImporter.h>

#include <dxgidebug.h>
#include <Logging/Log.h>

Microsoft::WRL::ComPtr<ID3D11Device> DX11Pipeline::device;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> DX11Pipeline::context;
Microsoft::WRL::ComPtr<ID3D11RasterizerState> DX11Pipeline::rasterizer;
Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DX11Pipeline::depthStencilState;
Microsoft::WRL::ComPtr<IWICImagingFactory> DX11Pipeline::ImageFactory;
Microsoft::WRL::ComPtr<ID3D11SamplerState> DX11Pipeline::SamplerState;

ShaderIncluder DX11Pipeline::Includer;

void DX11Pipeline::Init() {
	SF_LOG(Render Pipeline, Log, "Initializing DX11 Render Pipeline");

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1
	};

	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		deviceFlags,
		levels,
		ARRAYSIZE(levels),
		D3D11_SDK_VERSION,
		&device,
		&featureLevel,
		&context
	);

	if (FAILED(result)) {
		SF_LOG(Render Pipeline, Error, "Failed to create dx11 device")
	}

	// variables to hold the current rasterizer state and its description
	D3D11_RASTERIZER_DESC rDesc;
	
	//rDesc.FillMode = D3D11_FILL_WIREFRAME; // change the ONE setting
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.CullMode = D3D11_CULL_BACK;
	rDesc.FrontCounterClockwise = false;
	rDesc.DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
	rDesc.DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
	rDesc.SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rDesc.DepthClipEnable = true;
	rDesc.ScissorEnable = false;
	rDesc.MultisampleEnable = false;
	rDesc.AntialiasedLineEnable = false;
	
	// create a whole new rasterizer state
	// d3d is the ID3D11Device
	DX11Pipeline::device->CreateRasterizerState(&rDesc, &rasterizer);
	
	context->RSSetState(rasterizer.Get());    // set the new rasterizer state

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	depthStencilStateDesc.DepthEnable = TRUE; // TRUE
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;
	depthStencilStateDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	
	// Stencil operations if pixel is front-facing
	depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	// Stencil operations if pixel is back-facing
	depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	
	device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
	context->OMSetDepthStencilState(depthStencilState.Get(), 0);

	D3D11_SAMPLER_DESC SamplerDesc;
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = SamplerDesc.BorderColor[1] = SamplerDesc.BorderColor[2] = SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&SamplerDesc, &SamplerState);

	context->PSSetSamplers(0, 1, SamplerState.GetAddressOf());


	CoInitialize(NULL);

	result = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&ImageFactory)
	);

	SetupDefaultMaterial();
	SetupErrorMaterial();
}

void DX11Pipeline::Shutdown() {
	DefaultMaterial = nullptr;
	ErrorMaterial = nullptr;
	ImageFactory.Reset();
	
	CoUninitialize();

	SamplerState.Reset();
	depthStencilState.Reset();
	rasterizer.Reset();
	context.Reset();
	device.Reset();

#if defined(DEBUG) || defined(_DEBUG)
	IDXGIDebug* debugDev;
	HRESULT hr = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debugDev));
	debugDev->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
#endif
}

SFSharedPtr<Window> DX11Pipeline::CreateWindow(const WindowParams& params) {
	DX11Window* window = new DX11Window(params);

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.Windowed = TRUE;
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	desc.OutputWindow = window->GetHandle();

	Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;

	device.As(&dxgiDevice);

	HRESULT result = dxgiDevice->GetAdapter(&adapter);

	if (!SUCCEEDED(result)) {
		SF_LOG(Render Pineline, Error, "Failed to get adapter while creating window")
		delete window;
		return nullptr;
	}

	adapter->GetParent(IID_PPV_ARGS(&factory));

	result = factory->CreateSwapChain(device.Get(), &desc, &window->SwapChain);

	if (!SUCCEEDED(result)) {
		SF_LOG(Render Pineline, Error, "Failed to create swap chain while creating window: %i", GetLastError())
		delete window;
		return nullptr;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBuffer;

	result = window->SwapChain->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		(void**)&BackBuffer);

	D3D11_TEXTURE2D_DESC BackBufferDesc;
	BackBuffer->GetDesc(&BackBufferDesc);

	if (!SUCCEEDED(result)) {
		SF_LOG(Render Pineline, Error, "Failed to get back buffer while creating window")
		delete window;
		return nullptr;
	}
	
	result = device->CreateRenderTargetView(BackBuffer.Get(), nullptr, window->RenderTarget.GetAddressOf());

	if (!SUCCEEDED(result)) {
		SF_LOG(Render Pineline, Error, "Failed to create render target view while creating window")
		delete window;
		return nullptr;
	}

	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT>(BackBufferDesc.Width),
		static_cast<UINT>(BackBufferDesc.Height),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);

	result = device->CreateTexture2D(&depthStencilDesc, nullptr, &window->DepthStencil);

	if (!SUCCEEDED(result)) {
		SF_LOG(Render Pineline, Error, "Failed to create depth stencil texture while creating window")
		delete window;
		return nullptr;
	}

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

	result = device->CreateDepthStencilView(
		window->DepthStencil.Get(),
		&depthStencilViewDesc,
		&window->DepthStencilView
	);

	if (!SUCCEEDED(result)) {
		SF_LOG(Render Pineline, Error, "Failed to create depth stencil resource while creating window")
		delete window;
		return nullptr;
	}

	ZeroMemory(&window->Viewport, sizeof(D3D11_VIEWPORT));
	window->Viewport.Height = BackBufferDesc.Height;
	window->Viewport.Width = BackBufferDesc.Width;
	window->Viewport.MinDepth = 0;
	window->Viewport.MaxDepth = 1;

	context->OMSetRenderTargets(1, window->RenderTarget.GetAddressOf(), window->DepthStencilView.Get());
	context->RSSetViewports(1, &window->Viewport);

	return window;
}

SFSharedRef<Mesh> DX11Pipeline::CreateMesh() {
	return SFSharedRef<Mesh>(new DX11Mesh());
}

SFSharedRef<Material> DX11Pipeline::GetDefaultMaterial() const {
	return DefaultMaterial;
}

SFSharedRef<Material> DX11Pipeline::GetErrorMaterial() const {
	return ErrorMaterial;
}

SFSharedRef<RenderTarget> DX11Pipeline::CreateRenderTarget(const Bounds2Di& bounds) {
	return nullptr;
}

void DX11Pipeline::BeginRender(const Camera& camera) {

}

void DX11Pipeline::EndRender(const Camera& camera) {

}

SFSharedRef<Shader> DX11Pipeline::CreateShader(ShaderType type) {
	return SFSharedRef<Shader>(new DX11Shader(type));
}

SFSharedRef<Texture2D> DX11Pipeline::CreateTexture2D() {
	return SFSharedRef<Texture2D>(new DX11Texture2D());
}

void DX11Pipeline::SetupDefaultMaterial() {
	DefaultMaterial = new DX11Material();
	SFSharedRef<Shader> vert = CreateShader(ShaderType::Vertex);
	SFSharedRef<Shader> frag = CreateShader(ShaderType::Fragment);
	ShaderImporter importer;
	importer.ImportFile("Shaders/Default/Vert.hlsl", *vert);
	importer.ImportFile("Shaders/Default/Frag.hlsl", *frag);

	DefaultMaterial->BindShaders({ vert, frag });
}

void DX11Pipeline::SetupErrorMaterial() {
	ErrorMaterial = new DX11Material();
	SFSharedRef<Shader> vert = CreateShader(ShaderType::Vertex);
	SFSharedRef<Shader> frag = CreateShader(ShaderType::Fragment);
	ShaderImporter importer;
	importer.ImportFile("Shaders/Error/Vert.hlsl", *vert);
	importer.ImportFile("Shaders/Error/Frag.hlsl", *frag);

	ErrorMaterial->BindShaders({ vert, frag });
}