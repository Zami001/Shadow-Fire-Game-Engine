#include <Sources/AudioTone.h>

void AudioTone::SetFrequency(unsigned long long NewFrequency) {
    Frequency = NewFrequency;
}

void AudioTone::ProcessAudio(ma_device* pDevice, void* pOutput, ma_uint32 frameCount) {
    switch (Type) {
        case ToneType::Sine:
            WriteToOutput(SineWave, pDevice, pOutput, frameCount);
            break;

        case ToneType::Square:
            WriteToOutput(SquareWave, pDevice, pOutput, frameCount);
            break;

        case ToneType::Sawtooth:
            WriteToOutput(SawtoothWave, pDevice, pOutput, frameCount);
            break;

        case ToneType::Triangle:
            WriteToOutput(TriangleWave, pDevice, pOutput, frameCount);
            break;
    }
}