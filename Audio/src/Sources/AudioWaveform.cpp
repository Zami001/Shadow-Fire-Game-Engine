#include <Sources/AudioWaveform.h>

void AudioWaveform::SetAmplitude(float NewAmplitude) {
    Amplitude = NewAmplitude;
}

void AudioWaveform::SetFrequency(unsigned long long NewFrequency) {
    Frequency = NewFrequency;
}

void AudioWaveform::ProcessAudio(ma_device* pDevice, void* pOutput, ma_uint32 frameCount) {
    switch (Type) {
        case Waveform::Sine:
            WriteToOutput(SineWave, pDevice, pOutput, frameCount);
            break;

        case Waveform::Square:
            WriteToOutput(SquareWave, pDevice, pOutput, frameCount);
            break;

        case Waveform::Sawtooth:
            WriteToOutput(SawtoothWave, pDevice, pOutput, frameCount);
            break;

        case Waveform::Triangle:
            WriteToOutput(TriangleWave, pDevice, pOutput, frameCount);
            break;
    }
}