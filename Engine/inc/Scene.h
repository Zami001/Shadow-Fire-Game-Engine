#pragma once

#include <Engine.h>
#include <vector>
#include <GameObject.h>
#include <SFObject.h>
#include <MinimalCore.h>

class Game;

class ENGINE_API Scene final : public SFObject {
	friend Game;

public:
	Scene();
	~Scene();

	std::vector<SFSharedPtr<GameObject>> objects;
	
	virtual void Serialize(SerializedAsset& asset) override;

	SFSharedPtr<GameObject> SpawnGameObjectDelayed() {
		SFSharedPtr<GameObject> spawned = ConstructObject<GameObject>();
		objects.push_back(spawned);
		spawned->scene = this;
		return spawned;
	}

	SFSharedPtr<GameObject> SpawnGameObject() {
		SFSharedPtr<GameObject> spawned = SpawnGameObjectDelayed();
		spawned->FinishConstruction();
		return spawned;
	}

	void Render(const Camera& camera);

private:
	std::vector<SFSharedRef<Scene>> Subscenes;

public:
	inline const std::vector<SFSharedRef<Scene>>& GetSubScenes() { return Subscenes; }
};