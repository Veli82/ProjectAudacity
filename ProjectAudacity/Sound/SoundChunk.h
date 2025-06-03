	#pragma once
#include "Sound.h"
#include <stdexcept>

//Proxy class
class SoundChunk
{
public:
	SoundChunk(Sound& sound, int startSample, int endSample)
		:sound(sound)
	{
		if (startSample < 0 || endSample < 0 || endSample < startSample || endSample >= sound.getNumOfSamples())
		{
			throw std::runtime_error("Invalid SoundChunk params passed!");
		}
		this->startSample = startSample;
		this->endSample = endSample;
		this->numOfSamples = endSample - startSample;
	}

	float getSample(int index) const
	{
		if (index < 0 || index >= numOfSamples)
		{
			throw std::runtime_error("Invalid index passed when trying to get a sound sample!");
		}
		return sound.getSample(startSample + index);
	}

	int getNumOfSamples() const
	{
		return numOfSamples;
	}

private:
	Sound& sound;			//make ref
	int startSample;
	int endSample;
	int numOfSamples;
	//these are indexes, relative to the Sound* sound
};
