#pragma once

#include <Audio.h>
#include <miniaudio.h>
#include <Sources/AudioSource.h>

class AUDIO_API AudioClip : public AudioSource {
public:
	AudioClip() = default;
	~AudioClip() = default;

protected:
	virtual void ProcessAudio(ma_device* pDevice, void* pOutput, ma_uint32 frameCount) override;
};