#pragma once
#include "GeneratedSound.h"

class Chirp : public GeneratedSound
{
public:
	Chirp(double duration, int sampleRate, float amplitude, int freqStart, int freqEnd);
	float getSample(int index) override;
protected:
	float amplitude;
	int freqStart;
	int freqEnd;
	float freqSlope;
};

