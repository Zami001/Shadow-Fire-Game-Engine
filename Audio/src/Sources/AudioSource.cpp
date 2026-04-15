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
