#include <VertexBuffer.h>

VertexBuffer::VertexBuffer() : Indices(nullptr), VertexData(nullptr), VertexStride(0), NumIndices(0), NumVerts(0) {}

VertexBuffer::~VertexBuffer() {
	delete[] Indices;
	delete[] VertexData;
}

void VertexBuffer::SetIndices(const IndexType* InIndices, size_t InIndexCount) {
	delete[] Indices;
	Indices = new IndexType[InIndexCount];
	memcpy(Indices, InIndices, sizeof(IndexType) * InIndexCount);
	NumIndices = InIndexCount;
}

void VertexBuffer::SetVertexData(const void* InVertexData, size_t InVertexCount) {
	delete[] VertexData;
	VertexData = new uint8_t[VertexStride * InVertexCount];
	memcpy(VertexData, InVertexData, InVertexCount * VertexStride);
	NumVerts = InVertexCount;
}

void VertexBuffer::SetDescriptors(const std::vector<VertexDescriptor>& InDescriptors) {
	delete[] VertexData;
	VertexData = nullptr;

	Descriptors.clear();
	Descriptors = InDescriptors;

	VertexStride = 0;
	for (int i = 0; i < InDescriptors.size(); ++i) {
		VertexStride += GetDescriptorSize(InDescriptors[i]);
	}
}

void VertexBuffer::SetTopology(Topology NewTopology) {
	topology = NewTopology;
}

static_assert(VertexBuffer::GetDescriptorSize(VertexBuffer::VertexDescriptor::Bone_Index) == sizeof(VertexBuffer::BoneIndex) * 4, "Bone index descriptor size doesn't match");
static_assert(VertexBuffer::GetDescriptorSize(VertexBuffer::VertexDescriptor::Bone_Weight) == sizeof(VertexBuffer::BoneWeight) * 4, "Bone weight descriptor size doesn't match");