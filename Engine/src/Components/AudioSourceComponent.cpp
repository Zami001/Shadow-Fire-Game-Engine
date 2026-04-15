#include <Components/AudioSourceComponent.h>
#include <Game.h>
#include <Sources/AudioClip.h>

AssetType<AudioSourceComponent> assetType;

void AudioSourceComponent::Serialize(SerializedAsset& asset) {
	Component::Serialize(asset);

	asset.SetAssetType<AudioSourceComponent>();
}

void AudioSourceComponent::PlayAudio() {
	Source->PlayAudio(GetGameInstance()->GetAudioEngine());
}

void AudioSourceComponent::LoadAudio(const char* File) {
	if (!File) {
		return;
	}

	AudioClip* Clip = new AudioClip();
	Clip->LoadAudio(File, GetGameInstance()->GetAudioEngine());
	Source = Clip;
}

void AudioSourceComponent::Initialize() {
	Component::Initialize();
}
