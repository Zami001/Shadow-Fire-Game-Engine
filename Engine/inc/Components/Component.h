#pragma once

#include <Engine.h>
#include <SFObject.h>

class GameObject;
class Scene;

class ENGINE_API Component : public SFObject {
public:
	Component();
	~Component();

	Component(const Component&) = delete;
	Component(Component&&) = delete;

	GameObject* GetAttachedTo() { return AttachedTo; }
	void AttachTo(GameObject* object);

	Scene* GetScene() const;

	virtual void Serialize(SerializedAsset& asset) override;

private:
	GameObject* AttachedTo;
};