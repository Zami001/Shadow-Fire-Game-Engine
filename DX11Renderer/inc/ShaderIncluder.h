#pragma once

#include <DX11Renderer.h>
#include <MinimalCore.h>

#include <D3dcommon.h>

class DX11_API ShaderIncluder : public ID3DInclude {
public:
	// due to how d3dcompile works, cacheing using reference counting with each open/close won't work
	virtual __declspec(nothrow) HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;
	virtual __declspec(nothrow) HRESULT Close(LPCVOID pData) override;
};