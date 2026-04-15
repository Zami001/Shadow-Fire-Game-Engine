#pragma once

#include <Engine.h>
#include <Components/Component.h>
#include <Sources/AudioSource.h>

class ENGINE_API AudioSourceComponent : public Component {
public:
	AudioSourceComponent() = default;
	~AudioSourceComponent() = default;

	virtual void Serialize(SerializedAsset& asset) override;

	void PlayAudio();

	void LoadAudio(const char* File);

protected:
	virtual void Initialize() override;
	//virtual void Tick(float DeltaTime) override;

private:
	SFSharedPtr<AudioSource, true> Source;
};