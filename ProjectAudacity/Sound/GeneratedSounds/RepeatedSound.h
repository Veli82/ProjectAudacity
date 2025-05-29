#pragma once
#include "GeneratedSound.h"

class RepeatedSound : public GeneratedSound
{
public:
	RepeatedSound(int sampleRate, Sound* soundToRepeat, int timesRepeated);

	float getSample(int index) const override;

protected:
	Sound* soundToRepeat;
	int timesRepeated;
};

