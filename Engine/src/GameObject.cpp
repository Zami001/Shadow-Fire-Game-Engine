#include <GameObject.h>

AssetType<GameObject> assetType;

GameObject::GameObject() : RootComponent(nullptr), scene(nullptr) {}

GameObject::~GameObject() {}

void GameObject::Render(const Camera& camera) {
	if (RootComponent.Get()) {
		RootComponent->Render(camera, RootComponent->transform.GetLocalMatrix());
	}
}

void GameObject::Serialize(SerializedAsset& asset) {
	SFObject::Serialize(asset);

	asset.SetAssetType<GameObject>();

	SERIALIZE_SUBOBJECT_COLLECTION(asset, Components);
	//for (int i = 0; i < Components.size(); ++i) {
	//	asset.SerializeSubobject(*Components[i]);
	//}

	if (RootComponent.Get()) {
		asset.SerializeSubobject(*RootComponent, "RootComponent");
	}
}