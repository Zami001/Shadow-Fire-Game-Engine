#pragma once

#include <Audio.h>
#include <miniaudio.h>
#include <Sources/AudioSource.h>
#include <string>

class AUDIO_API AudioClip : public AudioSource {
public:
	AudioClip();
	~AudioClip();

	bool LoadAudio(const char* file, AudioEngine& Engine);

	bool IsLooping() const override;
	void SetLooping(bool loop) override;

protected:
	virtual void ProcessAudio(ma_device* pDevice, void* pOutput, ma_uint32 frameCount) override;
	virtual void PrePlay(AudioEngine& Engine) override;

private:
	//ma_decoder decoder;
	ma_sound sound;

#if SF_DEBUG
	std::string Filename;
#endif

	static void OnSoundEnded(void* pUserData, ma_sound* pSound);
};