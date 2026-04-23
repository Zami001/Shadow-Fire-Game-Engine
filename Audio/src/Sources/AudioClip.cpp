#include <Sources/AudioClip.h>

AudioClip::AudioClip() : AudioSource(false) {
}

AudioClip::~AudioClip() {
#if SF_DEBUG
	SF_LOG(Audio, Verbose, "Sound has been deleted: %s", Filename.c_str());
#else
	SF_LOG(Audio, Verbose, "Sound has been deleted");
#endif
	//ma_decoder_uninit(&decoder);
	//ma_sound_uninit(&sound);
}

void AudioClip::ProcessAudio(ma_device* pDevice, void* pOutput, ma_uint32 frameCount) {
	//ma_uint64 readFrames;
	//ma_decoder_read_pcm_frames(&decoder, pOutput, frameCount, &readFrames);
	//
	//if (readFrames < frameCount) {
	//	HasStopped = true;
	//}
}

void AudioClip::PrePlay(AudioEngine& Engine) {
	//ma_decoder_config config;

	//ma_decoder_config_init(ma_format_f32, GetDevice(Engine).playback.channels, GetDevice(Engine).sampleRate);
	//ma_decoder_init_file("Test Assets/Bloop.wav", &config, &decoder);
	
	if (Playing || sound.pDataSource != nullptr) {
#if SF_DEBUG
		SF_LOG(Audio, VeryVerbose, "Seeking to start: %s", Filename.c_str());
#else
		SF_LOG(Audio, VeryVerbose, "Seeking to start");
#endif
		ma_sound_seek_to_pcm_frame(&sound, 0);
		//return;
	} else {
		SF_LOG(Audio, Warning, "Sound has not been set");
		return;
	}
	
	sound.pEndCallbackUserData = this;
	sound.endCallback = OnSoundEnded;

	ma_sound_start(&sound);

	//ma_engine_play_sound(&GetEngine(Engine), "Test Assets/Bloop.wav", NULL);
}

bool AudioClip::LoadAudio(const char* file, AudioEngine& Engine) {
	bool result = ma_sound_init_from_file(&GetEngine(Engine), file, 0, NULL, NULL, &sound) != MA_SUCCESS;

	if (!result) {
#if SF_DEBUG
		Filename = file;
#endif
	
		SF_LOG(Audio, Verbose, "Sound file loaded: %s", file);
	}

	return result;
}

bool AudioClip::IsLooping() const {
	return ma_sound_is_looping(&sound);
}

void AudioClip::SetLooping(bool loop) {
	ma_sound_set_looping(&sound, loop);
}

void AudioClip::OnSoundEnded(void* pUserData, ma_sound* pSound) {
	AudioClip* Self = static_cast<AudioClip*>(pUserData);
	Self->Playing = false;
	ma_sound_stop(pSound);
	
#if SF_DEBUG
	SF_LOG(Audio, VeryVerbose, "Sound has ended: %s", Self->Filename.c_str());
#else
	SF_LOG(Audio, VeryVerbose, "Sound has ended");
#endif
}