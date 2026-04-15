#pragma once

#include <Audio.h>
#include <atomic>
#include <miniaudio.h>
#include <AudioEngine.h>

class AudioEngine;

class AUDIO_API AudioSource : public SharedFromThis<AudioSource, true> {
	friend AudioEngine;

public:
	AudioSource() = delete;
	virtual ~AudioSource() = default;

	void PlayAudio(AudioEngine& Engine);
	void StopAudio();

private:
	bool IsRawStream;

protected:
	AudioSource(bool RawStream) : IsRawStream(RawStream) {}

	std::atomic<bool> Playing = false;

	// Only called for raw stream audio sources
	// Add the audio signal to the pOutput buffer.
	// output needs to have frameCount x No. of channels, found in pDevice->playback.channels
	// data is organised sequentially by channel, then frame
	// data in the output buffer should be added to the existing value, not set directly.
	virtual void ProcessAudio(ma_device* pDevice, void* pOutput, ma_uint32 frameCount) {};

	// Called to initialize the sounds with the audio engine and begin playing the sound in the miniaudio engine if applicable
	virtual void PrePlay(AudioEngine& Engine) {}

	inline ma_engine& GetEngine(AudioEngine& Engine) { return Engine.engine; }
	inline ma_device& GetDevice(AudioEngine& Engine) { return Engine.device; }
};