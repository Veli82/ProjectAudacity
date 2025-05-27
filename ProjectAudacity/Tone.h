#pragma once
#include "GeneratedSound.h"

class Tone : public GeneratedSound
{
public:
	Tone(double duration, int sampleRate, int waveFrequency, float amplitude);
	float getSample(int index) override;
protected:
	float amplitude;
	int waveFrequency;
};