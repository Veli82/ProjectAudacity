#pragma once
#include "../Sound.h"

class Silence : public Sound
{
public:
	Silence(double duration, int sampleRate);
	float getSample(int index) override;
};

