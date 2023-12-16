#include <Components/SkeletalMeshComponent.h>

SkeletalMeshComponent::SkeletalMeshComponent() : Time(0.f) {}

SkeletalMeshComponent::~SkeletalMeshComponent() {}

void SkeletalMeshComponent::Render(const Camera& camera, const Matrix4x4& worldMatrix) {
	MeshComponent::Render(camera, worldMatrix);

	Game* game = GetGameInstance();
	if (Animation.Get()) {
		GetMesh()->RenderSkeletal(camera.GetProjectionMatrix() * worldMatrix, GetMaterial(), *Animation, Time);
	} else {
		GetMesh()->Render(camera.GetProjectionMatrix() * worldMatrix, GetMaterial());
	}
}

void SkeletalMeshComponent::Serialize(SerializedAsset& asset) {
	MeshComponent::Serialize(asset);

	asset.SetAssetType<SkeletalMeshComponent>();
}

void SkeletalMeshComponent::Initialize() {
	MeshComponent::Initialize();

	EnableTick();
}

void SkeletalMeshComponent::Tick(float DeltaTime) {
	if (Animation.Get()) {
		Time = std::fmodf(DeltaTime * Animation->GetPlaySpeed() + Time, Animation->GetDuration());
	}
}
