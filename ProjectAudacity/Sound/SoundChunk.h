#pragma once
#include "Sound.h"

//tova mozhe i da se implementira kato proxy class no mislq che ne e dobra ideq
class SoundChunk
{
public:
	SoundChunk(Sound& sound, int startSample, int endSample)
	{
		this->sound = &sound;
		this->startSample = startSample;
		this->endSample = endSample;
		this->numOfSamples = endSample - startSample;
	}

	float getSample(int index) const
	{
		return sound->getSample(startSample + index);
	}

	int getNumOfSamples() const
	{
		return numOfSamples;
	}

private:
	Sound* sound;
	int startSample;
	int endSample;
	int numOfSamples;
	//these are indexes, relative to the Sound* sound
};
