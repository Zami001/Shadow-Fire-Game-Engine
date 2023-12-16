#pragma once

#include <Components/SceneComponent.h>
#include <Mesh.h>
#include <MinimalCore.h>

class ENGINE_API MeshComponent : public SceneComponent {
public:
	MeshComponent();
	~MeshComponent();

	virtual void Serialize(SerializedAsset& asset) override;

protected:
	virtual void Initialize() override;

private:
	SFSharedPtr<Mesh> mesh;
	SFSharedPtr<Material> mat;

public:
	void SetMaterial(SFSharedPtr<Material> NewMaterial);
	inline SFSharedPtr<Material> GetMaterial() const { return mat; }

	void SetMesh(SFSharedPtr<Mesh> NewMesh);
	inline SFSharedPtr<Mesh> GetMesh() const { return mesh; }
};