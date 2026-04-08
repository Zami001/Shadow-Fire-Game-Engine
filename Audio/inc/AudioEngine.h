#pragma once

#include <Audio.h>
//#include <miniaudio.h>
#include <Sources/AudioSource.h>
#include <MinimalCore.h>
#include <vector>
#include <concurrent_queue.h>

class AUDIO_API AudioEngine {
	friend AudioSource;

public:
	AudioEngine() = default;
	~AudioEngine() = default;

	bool InitAudio();
	void ShutdownAudio();

	void StartAudio();

	// Allows for housekeeping of the audio engine event calls and should be called on the main thread
	void Update();

	MulticastDelegate<AudioSource*> OnAudioStopped;

private:
	ma_device device;

	std::vector<AudioSource*> PlayingAudioSources;
	concurrency::concurrent_queue<AudioSource*> QueuedAudioSources;
	concurrency::concurrent_queue<AudioSource*> StoppedAudioSources;

	static void ProcessAudio(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
};