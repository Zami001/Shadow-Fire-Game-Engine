#pragma once

#include <DX11Renderer.h>
#include <VertexBuffer.h>
#include <d3d11.h>
#include <wrl.h>
#include <dxgi1_3.h>
#include <d3d11_3.h>

class DX11_API DX11VertexBuffer : public VertexBuffer {
	friend class DX11Mesh;

public:
	DX11VertexBuffer();
	virtual ~DX11VertexBuffer();

	virtual void SetIndices(const IndexType* Indices, size_t IndexCount) override;
	virtual void SetVertexData(const void* VertexData, size_t VertexCount) override;
	virtual void SetDescriptors(const std::vector<VertexDescriptor>& Descriptors) override;

	void Bind();
	const D3D11_INPUT_ELEMENT_DESC* GetLayoutDescription() const;

private:
	D3D11_INPUT_ELEMENT_DESC* Description;
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;

	static constexpr const char* DescriptorToSemantic(VertexDescriptor Descriptor);
	static constexpr DXGI_FORMAT DescriptorToFormat(VertexDescriptor Descriptor);
	static constexpr D3D_PRIMITIVE_TOPOLOGY ConvertTopology(Topology topology);
};
