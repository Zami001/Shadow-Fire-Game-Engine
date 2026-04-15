#pragma once

#include <Audio.h>
#include <MinimalCore.h>
#include <vector>
#include <miniaudio.h>
#include <concurrent_queue.h>

class AudioSource;

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
	ma_engine engine;

	std::vector<AudioSource*> PlayingAudioSources;
	concurrency::concurrent_queue<AudioSource*> QueuedAudioSources;
	concurrency::concurrent_queue<AudioSource*> StoppedAudioSources;

	static void ProcessAudio(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
};