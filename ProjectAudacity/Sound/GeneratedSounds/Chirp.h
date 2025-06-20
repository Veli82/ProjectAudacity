#pragma once
#include "GeneratedSound.h"

class Chirp : public GeneratedSound
{
public:
	Chirp(float duration, int sampleRate, float amplitude, int freqStart, int freqEnd);

	float getSample(int index) const override;

	void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const override;
protected:
	float amplitude;
	int freqStart;
	int freqEnd;
	float freqSlope;
};

