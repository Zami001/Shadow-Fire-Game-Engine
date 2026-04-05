#pragma once

#include <Engine.h>
#include <Components/Component.h>
#include <vector>
#include <Camera.h>
#include <Math/Transform.h>
#include <MinimalCore.h>

class ENGINE_API SceneComponent : public Component {
public:
	SceneComponent();
	~SceneComponent();

	std::vector<SFSharedRef<SceneComponent>> Subcomponents;
	Transform transform;
	
	virtual void Render(const Camera& camera, const Matrix4x4& worldMatrix) {
		SF_LOG(Test, Log, "Render: %s", this->GetName());

		for (SFSharedRef<SceneComponent>& component : Subcomponents) {
			SF_LOG(Test, Log, "Subcomponent: %s", component->GetName());
			component->Render(camera, worldMatrix * component->transform.GetLocalMatrix());
		}
	}

	SceneComponent* GetParent() const { return Parent; }
	void SetParent(SceneComponent* parent);

	bool Visible;

	virtual void Serialize(SerializedAsset& asset) override;

	SFSharedPtr<SceneComponent, false> AsShared() {
		auto shared = Component::AsShared();
		return *reinterpret_cast<SFSharedPtr<SceneComponent, false>*>(&shared);
	}

private:
	SceneComponent* Parent;
};