#pragma once

#include <Engine.h>
#include <Components/Component.h>
#include <vector>
#include <Camera.h>
#include <Math/Transform.h>

class ENGINE_API SceneComponent : public Component {
public:
	SceneComponent();
	~SceneComponent();

	std::vector<SFSharedRef<SceneComponent>> Subcomponents;
	Transform transform;
	
	virtual void Render(const Camera& camera, const Matrix4x4& worldMatrix) {
		for (SFSharedRef<SceneComponent>& component : Subcomponents) {
			component->Render(camera, worldMatrix * component->transform.GetLocalMatrix());
		}
	}

	SceneComponent* GetParent() const { return Parent; }
	void SetParent(SceneComponent* parent);

	bool Visible;

	virtual void Serialize(SerializedAsset& asset) override;

private:
	SceneComponent* Parent;
};