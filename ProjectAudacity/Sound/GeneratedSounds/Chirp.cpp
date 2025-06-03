#include "Chirp.h"
#include <cmath>

const float PI = 3.1415927f;

Chirp::Chirp(float duration, int sampleRate, float amplitude, int freqStart, int freqEnd)
	:GeneratedSound(duration, sampleRate)
{
	setAmplitude(this->amplitude, amplitude);
	setWaveFrequency(this->freqStart, freqStart);
	setWaveFrequency(this->freqEnd, freqEnd);

	freqSlope = (freqEnd - freqStart) / duration;
}

float Chirp::getSample(int index) const
{
	validateIndex(index);
	float t = (float)index / sampleRate;
	return amplitude * std::sin(2.0f * PI * (freqStart * t + 0.5f * freqSlope * t * t));
}
