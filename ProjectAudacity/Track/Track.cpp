#include "Track.h"
#include <cassert>

Track::Track(int sampleRate)
    :Sound(0, sampleRate)
{
}

void Track::addSound(const Sound& sound)
{
	SoundChunk chunk(sound);
	sounds.push_back(chunk);
	numOfSamples += sound.getNumOfSamples();
	duration += sound.getDuration();
}

void Track::addSound(const Sound& sound, SoundChunk& dest)
{
	assert(sound.getNumOfSamples() == dest.getNumOfSamples());
	dest.setNewSound(sound);
}


void Track::addSound(const Sound& sound, int startSampleOnTrack)		//kakvo shte stane ako se dobavi track v track?? (bukv mozhe i da raboti)
{
	assert(startSampleOnTrack < numOfSamples);	//should be validated from outside

	int endSampleOnTrack = startSampleOnTrack + sound.getNumOfSamples() - 1;
	int startSampleOnSound;
	int endSampleOnSound;
	SoundChunk* firstChunk = findSound(startSampleOnTrack, startSampleOnSound);
	SoundChunk* lastChunk = findSound(endSampleOnTrack, endSampleOnSound);
	SoundChunk copy(*firstChunk);	//for first case
	firstChunk->setEnd(startSampleOnSound);
	SoundChunk chunk(sound);
	std::vector<SoundChunk>::iterator firstChunkPos = sounds.begin() + (firstChunk - sounds.data());

	if (firstChunk == lastChunk)
	{
		std::vector<SoundChunk>::iterator it = sounds.insert(firstChunkPos + 1, chunk);
		copy.setStart(endSampleOnSound + 1);
		sounds.insert(it + 1, copy);
	}
	else if (firstChunk && lastChunk)
	{
			lastChunk->setStart(endSampleOnSound + 1);
			std::vector<SoundChunk>::iterator lastChunkPos = sounds.begin() + (lastChunk - sounds.data());
			sounds.erase(firstChunkPos + 1, lastChunkPos);
			sounds.insert(firstChunkPos + 1, chunk);
	}
	else if (firstChunk && !lastChunk)
	{
		sounds.erase(firstChunkPos + 1, sounds.end());
		sounds.push_back(sound);
		numOfSamples = endSampleOnTrack + 1;
		duration = (float)numOfSamples / sampleRate;
	}
	//else if (!firstChunk && !lastChunk)		(the silence should get handled outside)
	else
	{
		throw std::runtime_error("Invalid data in Track::addSound(), we should not be here");
	}

	removeEmptyChunks();
}

float Track::getSample(int sampleIndexOnTrack) const
{
    validateIndex(sampleIndexOnTrack);
	int sampleIndexOnSound = 0;
	return findSound(sampleIndexOnTrack, sampleIndexOnSound)->getSample(sampleIndexOnSound);
}

SoundChunk& Track::operator[](int index)
{
	assert(index >= 0 && index < sounds.size());
	return sounds[index];
}

const SoundChunk& Track::operator[](int index) const
{
	assert(index >= 0 && index < sounds.size());
	return sounds[index];
}

std::vector<SoundChunk> Track::getChunks(int startSampleOnTrack, int endSampleOnTrack) const
{
	std::vector<SoundChunk> chunks;
	int startSampleOnSound;
	int endSampleOnSound;
	const SoundChunk* firstChunk = findSound(startSampleOnTrack, startSampleOnSound);
	const SoundChunk* lastChunk = findSound(endSampleOnTrack, endSampleOnSound);
	std::vector<SoundChunk>::const_iterator firstChunkPos = sounds.begin() + (firstChunk - sounds.data());
	std::vector<SoundChunk>::const_iterator lastChunkPos = sounds.begin() + (lastChunk - sounds.data());

	for (std::vector<SoundChunk>::const_iterator it = firstChunkPos; it <= lastChunkPos; it++ ) {
		chunks.push_back(*it);
	}

	chunks[0].setStart(startSampleOnSound);
	chunks[chunks.size() - 1].setEnd(endSampleOnSound);

	return chunks;
}

SoundChunk* Track::findSound(int sampleIndexOnTrack, int& sampleIndexOnSound)
{
	sampleIndexOnSound = sampleIndexOnTrack;
	for (int i = 0; i < sounds.size(); i++)
	{
		int chunkSize = sounds[i].getNumOfSamples();
		if (sampleIndexOnSound < chunkSize)
		{
			return &sounds[i];
		}
		else
		{
			sampleIndexOnSound -= chunkSize;
		}
	}
	return nullptr;	//case where sampleIndexOnTrack > track.numOfSamples
}

const SoundChunk* Track::findSound(int sampleIndexOnTrack, int& sampleIndexOnSound) const
{
	sampleIndexOnSound = sampleIndexOnTrack;
	for (int i = 0; i < sounds.size(); i++)
	{
		int chunkSize = sounds[i].getNumOfSamples();
		if (sampleIndexOnSound < chunkSize)
		{
			return &sounds[i];
		}
		else
		{
			sampleIndexOnSound -= chunkSize;
		}
	}
	return nullptr;
}

void Track::removeEmptyChunks()
{
	for (std::vector<SoundChunk>::iterator it = sounds.begin(); it != sounds.end(); ) {
		if (it->getNumOfSamples() == 0)
		{
			it = sounds.erase(it);
		}
		else
		{
			++it;
		}
	}
}





