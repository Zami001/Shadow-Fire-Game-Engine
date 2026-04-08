#include <Sources/AudioSource.h>
#include <AudioEngine.h>

void AudioSource::PlayAudio(AudioEngine* Engine) {
	Engine->QueuedAudioSources.push(this);
}

void AudioSource::StopAudio() {
	HasStopped = true;
}
