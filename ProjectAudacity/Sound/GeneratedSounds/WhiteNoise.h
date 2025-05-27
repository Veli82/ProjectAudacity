#pragma once
#include "GeneratedSound.h"

class WhiteNoise : public GeneratedSound
{
public:
	WhiteNoise(double duration, int sampleRate, float amplitude);
	float getSample(int index) override;
protected:
	float amplitude;
};

