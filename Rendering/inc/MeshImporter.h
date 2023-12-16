#pragma once

#include <Rendering.h>
#include <FileImporter.h>
#include <VertexBuffer.h>
#include <Mesh.h>
#include <Skeleton.h>
#include <RenderPipeline.h>
#include <unordered_set>
#include <MinimalCore.h>

struct RENDERING_API ImportedMeshAsset {
	std::vector<SFSharedRef<Mesh>> Meshes;
	std::vector<SFSharedRef<Skeleton>> Skeletons;
	std::vector<SFSharedRef<SkeletalAnimation>> Animations;
};

class RENDERING_API MeshImporter : public FileImporter<ImportedMeshAsset> {
public:
	MeshImporter(RenderPipeline& pipeline);
	~MeshImporter() = default;

protected:
	virtual bool ConvertData(ImportedMeshAsset& target, const char* data, size_t length) override;

private:
	Skeleton::Bone ConvertSkeletalNode(struct aiNode& Node, std::unordered_set<std::string>& BoneNames);
	RenderPipeline& pipeline;
};