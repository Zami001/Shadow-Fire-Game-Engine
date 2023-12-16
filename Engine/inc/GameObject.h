#pragma once

#include <Engine.h>
#include <vector>
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Camera.h>
#include <SFObject.h>

class Scene;

class ENGINE_API GameObject final : public SFObject {
	friend Scene;
	friend SFObject;

private:
	GameObject();

public:
	~GameObject();

	GameObject(const GameObject&) = delete;
	GameObject(GameObject&&) = delete;

	void operator=(const GameObject&) = delete;
	void operator=(GameObject&&) = delete;

	std::vector<SFSharedRef<Component>> Components;
	SFSharedPtr<SceneComponent> RootComponent;

	Scene* GetScene() const { return scene; }

	template<typename T, typename... Args>
	std::enable_if_t<!std::is_base_of_v<SceneComponent, T> && std::is_base_of_v<Component, T>, SFSharedRef<T>> AddComponent(Args... args) {
		SFSharedRef<T> newComponent = ConstructObject<T, Args...>(args...);
		Components.push_back(newComponent);
		newComponent->FinishConstruction();
		return newComponent;
	}
	
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<SceneComponent, T>, SFSharedRef<T>> AddComponent(Args... args) {
		SFSharedRef<T> newComponent = ConstructObject<T, Args...>(args...);

		if (RootComponent.Get()) {
			newComponent->SetParent(RootComponent.Get());
		} else {
			RootComponent = newComponent;
		}

		newComponent->FinishConstruction();

		return newComponent;
	}

	void Render(const Camera& camera);

	virtual void Serialize(SerializedAsset& asset) override;

private:
	Scene* scene;
};