#include <Sources/AudioClip.h>

AudioClip::AudioClip() : AudioSource(false) {
}

AudioClip::~AudioClip() {
	SF_LOG(Audio, Warning, "Sound has been deleted");
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
		SF_LOG(Audio, Warning, "Sound is already playing");
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
	return ma_sound_init_from_file(&GetEngine(Engine), file, 0, NULL, NULL, &sound) != MA_SUCCESS;
}

void AudioClip::OnSoundEnded(void* pUserData, ma_sound* pSound) {
	AudioClip* Self = static_cast<AudioClip*>(pUserData);
	Self->Playing = false;
	ma_sound_stop(pSound);
	SF_LOG(Audio, Warning, "Sound has ended");
}