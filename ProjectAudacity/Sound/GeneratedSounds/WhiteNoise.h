#pragma once
#include "GeneratedSound.h"

class WhiteNoise : public GeneratedSound
{
public:
	WhiteNoise(float duration, int sampleRate, float amplitude);

	float getSample(int index) const override;

	void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const override;
protected:
	float amplitude;
};

