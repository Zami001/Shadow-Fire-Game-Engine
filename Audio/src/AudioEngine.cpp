#include <AudioEngine.h>

void AudioEngine::ProcessAudio(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    AudioEngine* Engine = reinterpret_cast<AudioEngine*>(pDevice->pUserData);

    // audio sources expect clean audio in output as base, so need to zero it out to begin with
    std::memset(pOutput, 0, sizeof(float) * frameCount * pDevice->playback.channels);

    // begin playing queued audio
    AudioSource* NewSource;
    while (Engine->QueuedAudioSources.try_pop(NewSource)) {
        Engine->PlayingAudioSources.push_back(NewSource);
    }

    // process the audio of currently playing sounds
    for (int i = 0; i < Engine->PlayingAudioSources.size(); ++i) {
        AudioSource*& Source = Engine->PlayingAudioSources[i];

        // handle stopped audio sources
        if (Source->HasStopped) {
            Engine->StoppedAudioSources.push(Source);
            Engine->PlayingAudioSources[i] = Engine->PlayingAudioSources[Engine->PlayingAudioSources.size() - 1];
            Engine->PlayingAudioSources.pop_back();
            continue;
        }

        Source->ProcessAudio(pDevice, pOutput, frameCount);
    }
}

bool AudioEngine::InitAudio() {
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
    config.playback.channels = 2;               // Set to 0 to use the device's native channel count.
    config.sampleRate = 48000;           // Set to 0 to use the device's native sample rate.
    config.dataCallback = ProcessAudio;   // This function will be called when miniaudio needs more data.
    config.pUserData = this;   // Can be accessed from the device object (device.pUserData).

    return ma_device_init(NULL, &config, &device) == MA_SUCCESS;
}

void AudioEngine::ShutdownAudio() {
    ma_device_uninit(&device);
}

void AudioEngine::StartAudio() {
    ma_device_start(&device);
}

void AudioEngine::Update() {
    AudioSource* Source;
    while (StoppedAudioSources.try_pop(Source)) {
        OnAudioStopped(Source);
    }
}