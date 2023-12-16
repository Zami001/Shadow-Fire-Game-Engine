#pragma once

#include <Components/StaticMeshComponent.h>

//AssetType<StaticMeshComponent> assetType;

StaticMeshComponent::StaticMeshComponent() {}

StaticMeshComponent::~StaticMeshComponent() {}

void StaticMeshComponent::Render(const Camera& camera, const Matrix4x4& worldMatrix) {
	MeshComponent::Render(camera, worldMatrix);

	GetMesh()->Render(camera.GetProjectionMatrix() * worldMatrix, GetMaterial());
}

void StaticMeshComponent::Serialize(SerializedAsset& asset) {
	MeshComponent::Serialize(asset);

	asset.SetAssetType<StaticMeshComponent>();
}