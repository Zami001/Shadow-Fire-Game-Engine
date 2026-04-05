#include <Components/SceneComponent.h>

AssetType<SceneComponent> assetType;

SceneComponent::SceneComponent() : Visible(true), Parent(nullptr), Subcomponents() {}

SceneComponent::~SceneComponent() {}

void SceneComponent::Serialize(SerializedAsset& asset) {
	Component::Serialize(asset);

	asset.SetAssetType<SceneComponent>();

	SERIALIZE_SUBOBJECT_COLLECTION(asset, Subcomponents);
	SERIALIZE_OBJECT_REF(asset, Parent);
}

void SceneComponent::Render(const Camera& camera, const Matrix4x4& worldMatrix) {
	if (Visible) {
		for (SFSharedRef<SceneComponent>& component : Subcomponents) {
			component->Render(camera, worldMatrix * component->transform.GetLocalMatrix());
		}
	}
}

void SceneComponent::SetParent(SceneComponent* NewParent) {
	if (NewParent == Parent) return;

	if (Parent) {
		for (int i = 0; i < Parent->Subcomponents.size(); ++i) {
			if (Parent->Subcomponents[i] == this) {
				Parent->Subcomponents.erase(Parent->Subcomponents.begin() + i);
				break;
			}
		}
	}
	
	Parent = NewParent;

	if (NewParent) {
		NewParent->Subcomponents.push_back(this);
	}
}