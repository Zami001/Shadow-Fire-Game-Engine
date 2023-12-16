#pragma once

#include <Components/MeshComponent.h>
#include <Mesh.h>
#include <MinimalCore.h>

class ENGINE_API StaticMeshComponent : public MeshComponent {
public:
	StaticMeshComponent();
	~StaticMeshComponent();

	virtual void Render(const Camera& camera, const Matrix4x4& worldMatrix) override;

	virtual void Serialize(SerializedAsset& asset) override;
};