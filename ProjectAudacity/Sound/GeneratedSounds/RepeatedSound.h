#pragma once
#include "GeneratedSound.h"
#include "../SoundChunk.h"

class RepeatedSound : public GeneratedSound
{
public:
	RepeatedSound(int sampleRate,const SoundChunk& soundToRepeat, int timesRepeated);

	float getSample(int index) const override;

protected:
	const SoundChunk soundToRepeat;
	int timesRepeated;
};

