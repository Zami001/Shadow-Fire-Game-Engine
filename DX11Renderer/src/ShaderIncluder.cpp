#include <ShaderIncluder.h>
#include <filesystem>
#include <fstream>

HRESULT ShaderIncluder::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) {
	std::filesystem::path path;

	switch (IncludeType) {
		case D3D_INCLUDE_LOCAL: // #include "file.hlsl"
			path = std::filesystem::current_path() / "Shaders\\Includes\\" / pFileName;
			break;

		case D3D_INCLUDE_SYSTEM: // #include <file.hlsl>
			path = std::filesystem::current_path() / "Shaders\\Includes\\" / pFileName;
			break;

		default:
			return E_FAIL;
	}

	if (!std::filesystem::exists(path)) {
		SF_LOG(LogShaderInclude, Warning, "Shader include file doesn't exist: %s", path.string().c_str())
		return E_FAIL;
	}

	std::ifstream file;

	file.open(path.c_str(), std::ifstream::in | std::ifstream::binary);

	file.seekg(0, file.end);
	*pBytes = file.tellg();
	file.seekg(0, file.beg);

	char* data = new char[*pBytes];

	file.read(data, *pBytes);
	*ppData = data;

	return S_OK;
}

HRESULT ShaderIncluder::Close(LPCVOID pData) {
	delete[] const_cast<void*>(pData);
	return S_OK;
}
