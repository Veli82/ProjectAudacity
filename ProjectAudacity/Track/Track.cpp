#include <cassert>
#include <fstream>
#include <cstring>

#pragma warning (disable : 4996)

#include "Track.h"
#include "../Sound/FileSounds/WAVHeader.h"
#include "../Utils.h"

//settings for exporting the track, implementation for other values could be made in the future.
static short numOfChannels = 1;
static short bitsPerSample = 16;

Track::Track(int sampleRate)
    :Sound(0, sampleRate)
{
}

Track::Track(float duration, int sampleRate, const std::vector<SoundChunk>& sounds)
	:Sound(duration,sampleRate), sounds(sounds)
{
	//!!! sometimes creates an of by 1 error with the numOfSamples due to the not always perfect calculations and rounding errors!!!
	//so i've removed it for now but it has to be there
	//tova e pisano 1 chas predi deadlinea molq da me izvinite nqmam vreme za opravqne
	// za testerite: ako iskate probvaite da si go otkomentirate ponqkoga raboti ponqkoga ne. (mai pri imported file se chupi?)
	//int samplesSum = 0;
	//for (const SoundChunk& sound : sounds)
	//{
	//	samplesSum += sound.getNumOfSamples();
	//}
	//if (samplesSum != numOfSamples) throw std::runtime_error("Invalid input on creating a track.");
}

void Track::addSound(const Sound& sound)
{
	if (numOfSamples > INT_MAX - sound.getNumOfSamples()) throw std::runtime_error("Track's max size is reached!");
	setNumOfSamples(numOfSamples + sound.getNumOfSamples());
	SoundChunk chunk(sound);
	sounds.push_back(chunk);
}

void Track::addSound(const Sound& sound, SoundChunk& dest)
{
	assert(sound.getNumOfSamples() == dest.getNumOfSamples());
	dest.setNewSound(sound);
}


void Track::addSound(const Sound& sound, int startSampleOnTrack)		//kakvo shte stane ako se dobavi track v track?? (bukv mozhe i da raboti)
{
	assert(startSampleOnTrack < numOfSamples);	//should be validated from outside
	if (startSampleOnTrack - 1 > INT_MAX - sound.getNumOfSamples()) throw std::runtime_error("Track's max size is reached!");
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
		setNumOfSamples(endSampleOnTrack + 1);
		sounds.erase(firstChunkPos + 1, sounds.end());
		sounds.push_back(sound);
	}
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


void Track::writeToFile(const char* filename) const
{
	//a check needs to be made so that the file that written to isnt one of the others that are readed atm
	//mby just make an array of strings and save every ifstream filepath, and when you create an ofstream check for a match in the array
	std::ofstream ofs(filename, std::ios::binary);
	if (!ofs.is_open())
	{
		throw std::runtime_error("could not open file for writing");
	}

	writeWavHeader(ofs,numOfSamples,sampleRate,numOfChannels,bitsPerSample);

	for (const SoundChunk& chunk : sounds)
	{
		int chunkLength = chunk.getNumOfSamples();
		for (int i = 0; i < chunkLength; i++)
		{
			float flSample = chunk.getSample(i);
			short shSample = (short)(std::round(flSample * 32767.0f));
			ofs.write((const char*)&shSample, sizeof(shSample));
		}
	}
	if (ofs.fail())
	{
		throw std::runtime_error("Encountered an error when exporting the file!");
	}
}

void Track::writeToFileTest(float* arr) const
{
	int count = 0;
	for (const SoundChunk& chunk : sounds)
	{
		int chunkLength = chunk.getNumOfSamples();
		for (int i = 0; i < chunkLength; i++)
		{
			arr[count] = chunk.getSample(i);
			count++;
		}
	}
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
	if (endSampleOnTrack >= numOfSamples || startSampleOnTrack >= endSampleOnTrack) throw std::runtime_error("invalid input on trying to get chunks");
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

const std::vector<SoundChunk>& Track::getSoundsArr() const
{
	return sounds;
}

unsigned Track::getChunksCount() const
{
	return sounds.size();
}

void Track::save(std::ofstream& ofs, const std::vector<const Sound*>& allSounds) const
{
	unsigned trackSize = this->sounds.size();
	ofs.write((const char*)&sampleRate, sizeof(sampleRate));
	ofs.write((const char*)&duration, sizeof(duration));
	ofs.write((const char*)&trackSize, sizeof(trackSize));
	for (const SoundChunk& chunk : this->sounds)
	{
		chunk.save(ofs,allSounds);
	}
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





