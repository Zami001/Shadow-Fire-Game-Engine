#include <Components/CameraComponent.h>
#include <Game.h>
#include <Scene.h>

AssetType<CameraComponent> assetType;

void CameraComponent::Serialize(SerializedAsset& asset) {
	SceneComponent::Serialize(asset);

	asset.SetAssetType<CameraComponent>();
}

void CameraComponent::Initialize() {
	SceneComponent::Initialize();

	EnableTick(TickStage::Render);
}

void CameraComponent::Tick(float DeltaTime) {
	SceneComponent::Tick(DeltaTime);

	// todo: update camera position of the matrix using the transform of the camera component

	// Hack: Copy of scene pointers is being made. Could const cast away the const to render the scene but the list of scenes should be relatively small, so reducing risks by duplicating instead unless performance impact proves significant enough to optimise
	auto Scenes = GetGameInstance()->GetScenes();
	for (int i = 0; i < Scenes.size(); ++i) {
		Scenes[i]->Render(camera);
	}
}
