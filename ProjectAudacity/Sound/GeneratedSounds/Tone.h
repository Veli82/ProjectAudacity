#pragma once
#include "GeneratedSound.h"

class Tone : public GeneratedSound
{
public:
	Tone(float duration, int sampleRate, int waveFrequency, float amplitude);
	float getSample(int index) const override;
protected:
	float amplitude;
	int waveFrequency;
};