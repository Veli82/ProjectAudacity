#pragma once
#include "Sound.h"

struct SoundChunk
{
	SoundChunk(Sound& sound, int startSample, int endSample)
	{
		this->sound = &sound;
		this->startSample = startSample;
		this->endSample = endSample;
		this->numofSamples = endSample - startSample;
	}

	Sound* sound;
	int startSample;
	int endSample;
	int numofSamples;
	//these are indexes, relative to the Sound* sound
};
