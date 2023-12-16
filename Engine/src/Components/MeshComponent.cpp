#include <Components/MeshComponent.h>
#include <Game.h>

MeshComponent::MeshComponent() : mesh(nullptr), mat(nullptr) {}

MeshComponent::~MeshComponent() {}

void MeshComponent::Serialize(SerializedAsset& asset) {
	SceneComponent::Serialize(asset);

	asset.SetAssetType<MeshComponent>();
}

void MeshComponent::Initialize() {
	SceneComponent::Initialize();

	mat = GetGameInstance()->GetRenderer().GetDefaultMaterial();
}

void MeshComponent::SetMaterial(SFSharedPtr<Material> NewMaterial) {
	if (NewMaterial && NewMaterial->GetIsBound()) {
		mat = NewMaterial;
		return;
	}

	SF_LOG(LogMaterial, Warning, "Material was assigned that was not correctly initialized, assigning error material");
	mat = GetGameInstance()->GetRenderer().GetErrorMaterial();
}

void MeshComponent::SetMesh(SFSharedPtr<Mesh> NewMesh) {
	mesh = NewMesh;
}