#include "Chirp.h"
#include <cmath>
#include "../readAndWriteUtils.h"

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

void Chirp::save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
{
	SoundType type = SoundType::Chirp;
	ofs.write((const char*)&type, sizeof(type));
	GeneratedSound::save(ofs, sounds);
	ofs.write((const char*)&amplitude, sizeof(amplitude));
	ofs.write((const char*)&freqStart, sizeof(freqStart));
	ofs.write((const char*)&freqEnd, sizeof(freqEnd));
}
