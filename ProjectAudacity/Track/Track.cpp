#include <cassert>
#include <fstream>
#include <iostream>	//mahni posle
#include <cstring>

#pragma warning (disable : 4996)

#include "Track.h"
#include "../Sound/FileSounds/WAVHeader.h"
#include "../Utils.h"

//settings for exporting the track, implementation for other values could be made in the future.
short numOfChannels = 1;
short bitsPerSample = 16;

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

void Track::writeWavHeader(std::ofstream& ofs) const
{
	int subchunk2Size = numOfSamples * numOfChannels * (bitsPerSample / 8);
	WAVHeader header;
	strncpy(header.riffHeader, "RIFF", 4);
	header.fileSize = 36 + subchunk2Size;
	strncpy(header.waveHeader, "WAVE", 4);
	strncpy(header.fmtHeader, "fmt ", 4);
	header.fmtChunkSize = 16;
	header.audioFormat = 1;
	header.numChannels = numOfChannels;
	header.sampleRate = sampleRate;
	header.byteRate = sampleRate * numOfChannels * (bitsPerSample / 8);
	header.blockAlign = numOfChannels * (bitsPerSample / 8);
	header.bitsPerSample = bitsPerSample;

	assert(sizeof(header) == 36);
		
	char subchunk2Header[4];
	strncpy(subchunk2Header, "data", 4);

	ofs.write((const char*)&header, sizeof(header));
	ofs.write((const char*)&subchunk2Header, sizeof(subchunk2Header));
	ofs.write((const char*)&subchunk2Size, sizeof(subchunk2Size));
}

void Track::writeToFile(const char* filename) const
{
	std::ofstream ofs(filename, std::ios::binary);
	if (!ofs.is_open())
	{
		throw std::runtime_error("could not open file for writing");
	}

	writeWavHeader(ofs);

	for (const SoundChunk& chunk : sounds)
	{
		int chunkLength = chunk.getNumOfSamples();
		for (int i = 0; i < chunkLength; i++)
		{
			float flSample = clamp(chunk.getSample(i),-1.0f,1.0f);
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





