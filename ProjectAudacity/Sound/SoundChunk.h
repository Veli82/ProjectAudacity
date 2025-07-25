#pragma once
#include "Sound.h"
#include <stdexcept>
#include "readAndWriteUtils.h"

//Proxy class
class SoundChunk
{
public:
	SoundChunk(const Sound& sound, int startSample, int endSample)
		:sound(&sound)
	{
		if (startSample < 0 || endSample < 0 || endSample < startSample || endSample >= sound.getNumOfSamples())
		{
			throw std::runtime_error("Invalid SoundChunk params passed!");
		}
		this->startSample = startSample;
		this->endSample = endSample;
		this->numOfSamples = endSample - startSample + 1;
	}

	SoundChunk(const Sound& sound)
		:sound(&sound)
	{
		startSample = 0;
		endSample = sound.getNumOfSamples() - 1;
		numOfSamples = sound.getNumOfSamples();
	}


	float getSample(int index) const
	{
		if (index < 0 || index >= numOfSamples)
		{
			throw std::runtime_error("Invalid index passed when trying to get a sound sample!");
		}
		return sound->getSample(startSample + index);
	}

	int getNumOfSamples() const
	{
		return numOfSamples;
	}

	const Sound* getSound() const
	{
		return sound;
	}

	void setNewSound(const Sound& sound)
	{
		this->sound = &sound;
		startSample = 0;
		endSample = sound.getNumOfSamples() - 1;
		numOfSamples = sound.getNumOfSamples();
	}

	void setStart(int sampleIndex)
	{
		startSample += sampleIndex;
		numOfSamples -= sampleIndex;
	}
	void setEnd(int sampleIndex)
	{
		endSample = startSample + sampleIndex - 1;
		numOfSamples = endSample + 1 - startSample;
	}

	void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
	{
		int index = getIndexOfSound(sounds,sound);
		ofs.write((const char*)&index, sizeof(index));
		ofs.write((const char*)&startSample, sizeof(startSample));
		ofs.write((const char*)&endSample, sizeof(endSample));
	}

private:
	const Sound* sound;
	int startSample;
	int endSample;
	int numOfSamples;
	//these are indexes, relative to the Sound* sound
};
