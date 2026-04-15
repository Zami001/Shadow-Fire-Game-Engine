#include <AudioEngine.h>

#include <Sources/AudioSource.h>

void AudioEngine::ProcessAudio(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    AudioEngine* Engine = reinterpret_cast<AudioEngine*>(pDevice->pUserData);

    ma_engine_read_pcm_frames(&Engine->engine, pOutput, frameCount, NULL);

    // begin playing queued audio
    AudioSource* NewSource;
    while (Engine->QueuedAudioSources.try_pop(NewSource)) {
        Engine->PlayingAudioSources.push_back(NewSource);
    }

    // process the audio of currently playing sounds
    for (int i = 0; i < Engine->PlayingAudioSources.size(); ++i) {
        AudioSource*& Source = Engine->PlayingAudioSources[i];

        // handle stopped audio sources
        if (!Source->Playing) {
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
    config.playback.format = ma_format_f32;     // Set to ma_format_unknown to use the device's native format.
    config.playback.channels = 0;               // Set to 0 to use the device's native channel count.
    config.sampleRate = 0;                      // Set to 0 to use the device's native sample rate.
    config.dataCallback = ProcessAudio;         // This function will be called when miniaudio needs more data.
    config.pUserData = this;                    // Can be accessed from the device object (device.pUserData).

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        return false;
    }

    ma_engine_config engine_config = ma_engine_config_init();
    engine_config.pDevice = &device;

    if (ma_engine_init(&engine_config, &engine) != MA_SUCCESS) {
        ma_device_uninit(&device);
        return false;
    }

    return true;
}

void AudioEngine::ShutdownAudio() {
    ma_device_uninit(&device);
    ma_engine_uninit(&engine);
}

void AudioEngine::StartAudio() {
    ma_device_start(&device);
    //ma_engine_start(&engine);
}

void AudioEngine::Update() {
    AudioSource* Source;
    while (StoppedAudioSources.try_pop(Source)) {
        OnAudioStopped(Source);
    }
}