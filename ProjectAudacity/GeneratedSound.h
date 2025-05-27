#pragma once
#include "Sound.h"

class GeneratedSound : public Sound
{
public:
	GeneratedSound(double duration, int sampleRate);

protected:
	void setAmplitude(float& destination, float amplitude);
	void setWaveFrequency(int& destination, int frequency);
};