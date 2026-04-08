#pragma once

#include <Audio.h>
#include <atomic>
#include <miniaudio.h>

class AudioEngine;

class AUDIO_API AudioSource {
	friend AudioEngine;

public:
	AudioSource() = default;
	virtual ~AudioSource() = default;

	void PlayAudio(AudioEngine* Engine);
	void StopAudio();

protected:
	// Add the audio signal to the pOutput buffer.
	// output needs to have frameCount x No. of channels, found in pDevice->playback.channels
	// data is organised sequentially by channel, then frame
	// data in the output buffer should be added to the existing value, not set directly.
	virtual void ProcessAudio(ma_device* pDevice, void* pOutput, ma_uint32 frameCount) = 0;

	std::atomic<bool> HasStopped = false;
};