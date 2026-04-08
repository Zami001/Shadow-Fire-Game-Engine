#pragma once

#include <Audio.h>
#include <Sources/AudioSource.h>

class AUDIO_API AudioEffect : public AudioSource {
public:
	AudioEffect() = default;
	~AudioEffect() = default;
};