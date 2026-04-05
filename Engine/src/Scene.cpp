#include <Scene.h>
#include <Game.h>

AssetType<Scene> assetType;

Scene::Scene() {}

Scene::~Scene() {}

void Scene::Serialize(SerializedAsset& asset) {
	SFObject::Serialize(asset);

	asset.SetAssetType<Scene>();

	SERIALIZE_SUBOBJECT_COLLECTION(asset, objects);
}

void Scene::Render(const Camera& camera) {
	GetGameInstance()->GetRenderer().BeginRender(camera);

	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->Render(camera);
	}

	for (int i = 0; i < Subscenes.size(); ++i) {
		Subscenes[i]->Render(camera);
	}

	GetGameInstance()->GetRenderer().EndRender(camera);
}