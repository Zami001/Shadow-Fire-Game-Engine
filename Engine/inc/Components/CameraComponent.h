#pragma once

#include <Engine.h>
#include <Components/SceneComponent.h>
#include <Camera.h>

class ENGINE_API CameraComponent : public SceneComponent {
public:
	CameraComponent() = default;
	~CameraComponent() = default;

	Camera camera;

	virtual void Serialize(SerializedAsset& asset) override;

protected:
	virtual void Initialize() override;
	virtual void Tick(float DeltaTime) override;

private:

};