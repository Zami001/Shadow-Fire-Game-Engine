#include <Components/Component.h>
#include <GameObject.h>

AssetType<Component> assetType;

Component::Component() : AttachedTo(nullptr) {}

Component::~Component() {}

Scene* Component::GetScene() const {
	return AttachedTo->GetScene();
}

void Component::Serialize(SerializedAsset& asset) {
	SFObject::Serialize(asset);

	asset.SetAssetType<Component>();
}