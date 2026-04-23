#include <Sources/AudioSource.h>
#include <AudioEngine.h>

void AudioSource::PlayAudio(AudioEngine& Engine) {
	PrePlay(Engine);

	Playing = true;

	if (IsRawStream) {
		Engine.QueuedAudioSources.push(this);
	}
}

void AudioSource::StopAudio() {
	Playing = false;
}

void AudioSource::SetLooping(bool loop) {
	SF_LOG(Audio, Warning, "Attempting to set looping to an audio source that does not support looping");
}
