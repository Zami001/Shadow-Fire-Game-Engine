#include <DX11Shader.h>
#include <d3dcompiler.h>
#include <Math/Matrix4x4.h>
#include <Math/Vector.h>
#include <DX11Pipeline.h>
#include <Logging/Log.h>

DX11Shader::DX11Shader(ShaderType type) : Shader(type), VertexShader() {

}

DX11Shader::~DX11Shader() {
	switch (GetShaderType()) {
		case ShaderType::Vertex:
			VertexShader.~ComPtr();
			break;

		case ShaderType::Fragment:
			PixelShader.~ComPtr();
			break;
	}
}

bool DX11Shader::CompileShader(const char* str) {
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob;
	
	HRESULT result = D3DCompile(
		str,
		strlen(str),
		NULL,
		NULL,
		&DX11Pipeline::Includer,
		"main",
		GetShaderType() == ShaderType::Vertex ? "vs_5_0" : "ps_5_0",
		D3DCOMPILE_OPTIMIZATION_LEVEL3,
		NULL,
		&CompiledShader,
		&ErrorBlob
	);

	if (!SUCCEEDED(result)) {
		SF_LOG(Shader Compiler, Error, "Failed to compile the shader: \n%s\n%s", str, (char*)ErrorBlob->GetBufferPointer())
		return false;
	}

	switch (GetShaderType()) {
		case ShaderType::Vertex: {
			result = DX11Pipeline::device->CreateVertexShader(CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), NULL, &VertexShader);
			if (!SUCCEEDED(result)) {
				SF_LOG(Shader Compiler, Error, "Failed to create vertex shader")
				return false;
			}

			break;
		}

		case ShaderType::Fragment: {
			HRESULT result = DX11Pipeline::device->CreatePixelShader(
				CompiledShader->GetBufferPointer(),
				CompiledShader->GetBufferSize(),
				NULL,
				&PixelShader
			);
			if (!SUCCEEDED(result)) {
				SF_LOG(Shader Compiler, Error, "Failed to create fragment shader")
				return false;
			}
			break;
		}
	}

	return true;
}

