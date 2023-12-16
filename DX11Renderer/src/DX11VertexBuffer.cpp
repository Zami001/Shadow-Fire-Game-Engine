#include <DX11VertexBuffer.h>
#include <DX11Pipeline.h>

#include <assert.h>

DX11VertexBuffer::DX11VertexBuffer() : Description(nullptr) {
	SetDescriptors({ VertexDescriptor::Position, VertexDescriptor::Color_RGB_Float });
}

DX11VertexBuffer::~DX11VertexBuffer() {
	delete[] Description;
}

void DX11VertexBuffer::SetIndices(const IndexType* Indices, size_t IndexCount) {
	VertexBuffer::SetIndices(Indices, IndexCount);

	CD3D11_BUFFER_DESC iDesc(
		sizeof(IndexType) * GetNumIndices(),
		D3D11_BIND_INDEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA iData;
	ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
	iData.pSysMem = GetIndices();
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	DX11Pipeline::device->CreateBuffer(
		&iDesc,
		&iData,
		&IndexBuffer
	);
}

void DX11VertexBuffer::SetVertexData(const void* InVertexData, size_t InVertexCount) {
	VertexBuffer::SetVertexData(InVertexData, InVertexCount);
	
	CD3D11_BUFFER_DESC vDesc(
		InVertexCount * GetVertexStride(),
		D3D11_BIND_VERTEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA vData;
	ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
	vData.pSysMem = GetVertexData();
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	DX11Pipeline::device->CreateBuffer(
		&vDesc,
		&vData,
		&VertBuffer
	);
}

void DX11VertexBuffer::SetDescriptors(const std::vector<VertexDescriptor>& Descriptors) {
	VertexBuffer::SetDescriptors(Descriptors);

	delete[] Description;
	Description = new D3D11_INPUT_ELEMENT_DESC[Descriptors.size()];

	unsigned int offset = 0;
	for (int i = 0; i < Descriptors.size(); ++i) {
		//switch (Descriptors[i]) {
		//	case VertexDescriptor::Bone_Index:
		//		Description[i] = { DescriptorToSemantic(Descriptors[i]), 0, DescriptorToFormat(Descriptors[i]), 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		//		Description[i] = { DescriptorToSemantic(Descriptors[i]), 0, DescriptorToFormat(Descriptors[i]), 1, offset + 4, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		//		Description[i] = { DescriptorToSemantic(Descriptors[i]), 0, DescriptorToFormat(Descriptors[i]), 2, offset + 8, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		//		Description[i] = { DescriptorToSemantic(Descriptors[i]), 0, DescriptorToFormat(Descriptors[i]), 3, offset + 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		//		break;
		//
		//	default:
		//		
		//		break;
		//}

		Description[i] = { DescriptorToSemantic(Descriptors[i]), 0, DescriptorToFormat(Descriptors[i]), 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		offset += GetDescriptorSize(Descriptors[i]);
	}
}

void DX11VertexBuffer::Bind() {
	UINT stride = GetVertexStride(); //sizeof(float) * 6;
	UINT offset = 0;

	DX11Pipeline::context->IASetVertexBuffers(
		0,
		1,
		VertBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	static_assert(sizeof(IndexType) & 0b110, "");
	DXGI_FORMAT IndexFormat;
	switch (sizeof(IndexType) * 8) {
		case 16:
			IndexFormat = DXGI_FORMAT_R16_UINT;
			break;

		case 32:
			IndexFormat = DXGI_FORMAT_R32_UINT;
			break;

		default:
			throw std::exception("This code path should never be hit");
	}

	DX11Pipeline::context->IASetIndexBuffer(
		IndexBuffer.Get(),
		IndexFormat,
		0
	);

	DX11Pipeline::context->IASetPrimitiveTopology(ConvertTopology(GetTopology()));
}

const D3D11_INPUT_ELEMENT_DESC* DX11VertexBuffer::GetLayoutDescription() const {
	return Description;
}

constexpr const char* DX11VertexBuffer::DescriptorToSemantic(VertexDescriptor Descriptor) {
	switch (Descriptor) {
		case VertexDescriptor::Color_RGBA:			return "COLOR";
		case VertexDescriptor::Color_RGB_Float:		return "COLOR";
		case VertexDescriptor::Color_RGBA_Float:	return "COLOR";
		case VertexDescriptor::Normal:				return "NORMAL";
		case VertexDescriptor::Position:			return "POSITION";
		case VertexDescriptor::Tangent:				return "TANGENT";
		case VertexDescriptor::UV_Coordinate:		return "TEXCOORD";
		case VertexDescriptor::Bone_Index:			return "BLENDINDICES";
		case VertexDescriptor::Bone_Weight:			return "BLENDWEIGHT";
		
		default:
			throw std::exception("DX11VertexBuffer::DescriptorToSemantic - Descriptior not within enum range");
	}
}

constexpr DXGI_FORMAT DX11VertexBuffer::DescriptorToFormat(VertexDescriptor Descriptor) {
	switch (Descriptor) {
		case VertexDescriptor::Color_RGBA:			return DXGI_FORMAT_R8G8B8A8_UINT;
		case VertexDescriptor::Color_RGB_Float:		return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexDescriptor::Color_RGBA_Float:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case VertexDescriptor::Normal:				return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexDescriptor::Position:			return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexDescriptor::Tangent:				return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexDescriptor::UV_Coordinate:		return DXGI_FORMAT_R32G32_FLOAT;
		case VertexDescriptor::Bone_Index:			return DXGI_FORMAT_R32G32B32A32_UINT;
		case VertexDescriptor::Bone_Weight:			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		default:
			throw std::exception("DX11VertexBuffer::DescriptorToFormat - Descriptior not within enum range");
	}
}

constexpr D3D_PRIMITIVE_TOPOLOGY DX11VertexBuffer::ConvertTopology(Topology topology) {
	switch (topology) {
		case Topology::Point:		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		case Topology::Line:		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		case Topology::Triangle:	return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		default:
			throw std::exception("DX11VertexBuffer::ConvertTopology - Failed to convert topology");
	}
}