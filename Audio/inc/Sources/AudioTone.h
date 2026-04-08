#pragma once

#include <Audio.h>
#include <Sources/AudioSource.h>

#include <cmath>
#include <MinimalCore.h>

class AudioEngine;

class AUDIO_API AudioTone : public AudioSource {
	friend AudioEngine;

public:
	enum class ToneType {
		Sine,
		Square,
		Sawtooth,
		Triangle
	};

	AudioTone() = default;
	~AudioTone() = default;

	ToneType Type = ToneType::Sine;

	void SetFrequency(unsigned long long NewFrequency);

protected:
	virtual void ProcessAudio(ma_device* pDevice, void* pOutput, ma_uint32 frameCount) override;
	
private:
	double CurrentPhase = 0.0;
	unsigned long long Frequency = 240;
	float Amplitude = 0.1f;

	typedef float(*WaveFunc)(double);

	inline static float SineWave(double phase) {
		return static_cast<float>(std::sin(phase * Math::TAU));
	}

	inline static float SquareWave(double phase) {
		return phase > 0.5 ? 0.0f : 1.0f;
	}

	inline static float SawtoothWave(double phase) {
		return static_cast<float>(phase);
	}

	inline static float TriangleWave(double phase) {
		return std::abs((phase - 0.5) * 2);
	}

	inline void WriteToOutput(WaveFunc Func, ma_device* pDevice, void* pOutput, ma_uint32 frameCount) {
		float* output = reinterpret_cast<float*>(pOutput);
		int Channels = pDevice->playback.channels;
		double DumbyData;

		double Increment = static_cast<double>(Frequency) / static_cast<double>(pDevice->sampleRate);

		for (int i = 0; i < frameCount; ++i) {
			CurrentPhase += Increment;
			CurrentPhase = std::modf(CurrentPhase, &DumbyData);

			float value = Func(CurrentPhase) * Amplitude;
			for (int y = 0; y < Channels; ++y) {
				output[i * Channels + y] += value;
			}
		}
	}
};