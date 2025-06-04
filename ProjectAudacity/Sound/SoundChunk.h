	#pragma once
#include "Sound.h"
#include <stdexcept>
#include <cmath>

//Proxy class
class SoundChunk
{
public:

	SoundChunk(const Sound& sound)
		:sound(sound)
	{
		this->startSample = 0;
		this->endSample = sound.getNumOfSamples() - 1;
		this->numOfSamples = sound.getNumOfSamples();
	}


	SoundChunk(const Sound& sound, int startSample, int endSample)
		:sound(sound)
	{
		if (startSample < 0 || endSample < 0 || endSample < startSample || endSample >= sound.getNumOfSamples())
		{
			throw std::runtime_error("Invalid SoundChunk params passed!");
		}
		this->startSample = startSample;
		this->endSample = endSample;
		this->numOfSamples = endSample - startSample + 1;
	}

	//FIX
	SoundChunk operator=(const SoundChunk& other)
	{
		//sound = other.sound;	:(((((
		this->startSample = other.startSample;
		this->endSample = other.endSample;
		this->numOfSamples = other.numOfSamples;
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

	void setStart(int sampleIndex)
	{
		startSample += sampleIndex;
		numOfSamples += sampleIndex;
	}
	void setEnd(int sampleIndex)
	{
		numOfSamples -= endSample - startSample + sampleIndex;
		endSample = startSample + sampleIndex;
	}

private:
	const Sound& sound;		//off napravi go s pointer atp
	int startSample;
	int endSample;
	int numOfSamples;
	//these are indexes, relative to the Sound* sound
};
