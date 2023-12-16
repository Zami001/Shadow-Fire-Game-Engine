#pragma once

#include <DX11Renderer.h>
#include <Shader.h>
#include <wrl.h>
#include <d3d11.h>

class DX11_API DX11Shader : public Shader {
	friend class DX11Mesh;
	friend class DX11RenderPipeline;

public:
	DX11Shader(ShaderType type);
	~DX11Shader();

protected:
	virtual bool CompileShader(const char* str) override;

private:
	Microsoft::WRL::ComPtr<ID3DBlob> CompiledShader;

	union {
		Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
	};
};