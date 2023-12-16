#pragma once

#include <Components/MeshComponent.h>
#include <Mesh.h>
#include <MinimalCore.h>

class ENGINE_API SkeletalMeshComponent : public MeshComponent {
public:
	SkeletalMeshComponent();
	~SkeletalMeshComponent();

	virtual void Render(const Camera& camera, const Matrix4x4& worldMatrix) override;

	virtual void Serialize(SerializedAsset& asset) override;

	inline SFSharedPtr<SkeletalAnimation> GetAnimation() const { return Animation; }
	inline void SetAnimation(SFSharedPtr<SkeletalAnimation>& NewAnimation) { Animation = NewAnimation; }

	inline void SetAnimation(SFSharedPtr<SkeletalAnimation> NewAnimation) {
		Animation = NewAnimation;
	}

protected:
	virtual void Initialize() override;
	virtual void Tick(float DeltaTime) override;

private:
	// don't add getter/setter for Time since it will be abstracted away when adding animation blending
	float Time = 0;
	SFSharedPtr<SkeletalAnimation> Animation;
};