#pragma once

#include <Rendering.h>

#include <vector>
#include <stdint.h>
#include <string>

class RENDERING_API VertexBuffer {
public:
	// update DX11 convertion functions if these typedefs change

	typedef uint32_t IndexType;
	typedef uint32_t BoneIndex;
	typedef float BoneWeight;

	enum class VertexDescriptor : unsigned char {
		Color_RGBA_Float,
		Color_RGB_Float,
		Color_RGBA,
		Position,
		Normal,
		Tangent,
		UV_Coordinate,
		UVW_Coordinate,
		Bone_Index,
		Bone_Weight
	};

	enum class Topology {
		Point,
		Line,
		Triangle
	};

	VertexBuffer();
	virtual ~VertexBuffer();

private:
	std::vector<VertexDescriptor> Descriptors;
	size_t VertexStride;
	uint8_t* VertexData;
	IndexType* Indices;
	size_t NumVerts;
	size_t NumIndices;
	Topology topology = Topology::Triangle;

public:
	inline size_t GetNumVertex() const { return NumVerts; }
	inline const std::vector<VertexDescriptor>& GetDescriptors() const { return Descriptors; }
	inline const uint8_t* GetVertexData() const { return VertexData; }
	inline size_t GetVertexStride() const { return VertexStride; }
	inline const IndexType* GetIndices() const { return Indices; }
	inline size_t GetNumIndices() const { return NumIndices; }
	inline Topology GetTopology() const { return topology; }
	
	virtual void SetIndices(const IndexType* Indices, size_t IndexCount);
	virtual void SetVertexData(const void* VertexData, size_t VertexCount);
	virtual void SetDescriptors(const std::vector<VertexDescriptor>& Descriptors);
	virtual void SetTopology(Topology NewTopology);
	
	static constexpr size_t GetDescriptorSize(VertexDescriptor Descriptor);
};

constexpr size_t VertexBuffer::GetDescriptorSize(VertexDescriptor Descriptor) {
	switch (Descriptor) {
		case VertexDescriptor::Color_RGBA_Float:	return 16;
		case VertexDescriptor::Color_RGB_Float:		return 12;
		case VertexDescriptor::Color_RGBA:			return 4;
		case VertexDescriptor::Normal:				return 12;
		case VertexDescriptor::Position:			return 12;
		case VertexDescriptor::Tangent:				return 12;
		case VertexDescriptor::UV_Coordinate:		return 8;
		case VertexDescriptor::UVW_Coordinate:		return 12;
		case VertexDescriptor::Bone_Index:			return sizeof(IndexType) * 4;
		case VertexDescriptor::Bone_Weight:			return sizeof(BoneWeight) * 4;

		default:
			return (~0);
	}
}