#include "readAndWriteUtils.h"
#include "SoundChunk.h"

#include <string>

#include "FileSounds/WAVSound.h"
#include "Silence/Silence.h"
#include "GeneratedSounds/Tone.h"
#include "GeneratedSounds/WhiteNoise.h"
#include "GeneratedSounds/Chirp.h"
#include "GeneratedSounds/RepeatedSound.h"
#include "EffectSounds/Amplify.h"
#include "EffectSounds/Crossfade.h"
#include "../Track/Track.h"



int getIndexOfSound(const std::vector<const Sound*>& sounds, const Sound* sound)
{
	for (int i = 0; i < sounds.size(); i++)
	{
		if (sounds[i] == sound)
		{
			return i;
		}
	}
	throw std::runtime_error("Could not find this sound");
}


//the correct way to implement this is probably to have a default c-tor for every object and then have this functionality as a method function.
//but this way i am able to reuse the param c-tor logic so yeah.
//I dont have much time at the moment (2:45 until the deadline to be specific)
Sound* readFromFileSoundFactory(std::ifstream& ifs, const std::vector<const Sound*>& sounds)
{
	SoundType type;
	ifs.read((char*)&type, sizeof(type));
	float duration = 0;
	int sampleRate = 0;
	switch (type)
	{
	case SoundType::WAVSound:
	{
		unsigned strLength = 0;
		ifs.read((char*)&strLength, sizeof(strLength));
		std::string fileName(strLength, '\0');
		ifs.read(&fileName[0], strLength);
		return new WAVSound(fileName);
	}
	case SoundType::AIFFSound:
		throw std::runtime_error("Cannot load this sound!");
	case SoundType::Silence:
	{
		ifs.read((char*)&duration, sizeof(duration));
		ifs.read((char*)&sampleRate, sizeof(sampleRate));
		return new Silence(duration, sampleRate);
	}
	case SoundType::Tone:
	{
		int waveFreq = 0;
		float amp = 0;
		ifs.read((char*)&duration, sizeof(duration));
		ifs.read((char*)&sampleRate, sizeof(sampleRate));
		ifs.read((char*)&waveFreq, sizeof(waveFreq));
		ifs.read((char*)&amp, sizeof(amp));
		return new Tone(duration, sampleRate, waveFreq, amp);
	}
	case SoundType::WhiteNoise:
	{
		float amp = 0;
		ifs.read((char*)&duration, sizeof(duration));
		ifs.read((char*)&sampleRate, sizeof(sampleRate));
		ifs.read((char*)&amp, sizeof(amp));
		return new WhiteNoise(duration, sampleRate, amp);
	}
	case SoundType::Chirp:
	{
		int waveFreq1 = 0;
		int waveFreq2 = 0;
		float amp = 0;
		ifs.read((char*)&duration, sizeof(duration));
		ifs.read((char*)&sampleRate, sizeof(sampleRate));
		ifs.read((char*)&amp, sizeof(amp));
		ifs.read((char*)&waveFreq1, sizeof(waveFreq1));
		ifs.read((char*)&waveFreq2, sizeof(waveFreq2));
		return new Chirp(duration, sampleRate, amp, waveFreq1, waveFreq2);
	}
	case SoundType::RepeatedSound:	
	{
		int timesRepeated = 0;
		ifs.read((char*)&sampleRate, sizeof(sampleRate));
		SoundChunk chunk = readFromFileSoundChunk(ifs,sounds);
		ifs.read((char*)&timesRepeated, sizeof(timesRepeated));
		return new RepeatedSound(sampleRate, chunk, timesRepeated);
	}
	case SoundType::Amplify:
	{
		float decibels = 0;
		int chunkCount = 0;
		ifs.read((char*)&duration, sizeof(duration));
		ifs.read((char*)&sampleRate, sizeof(sampleRate));
		ifs.read((char*)&chunkCount, sizeof(chunkCount));
		std::vector<SoundChunk> baseSounds;
		for (int i = 0; i < chunkCount; i++)
		{
			baseSounds.push_back(readFromFileSoundChunk(ifs, sounds));
		}
		ifs.read((char*)&decibels, sizeof(decibels));
		return new Amplify(duration, sampleRate, baseSounds, decibels);
	}
	case SoundType::HighPassFilter:
		throw std::runtime_error("Cannot load this sound!");
	case SoundType::Crossfade:
	{
		bool fade = false;
		int chunkCount = 0;
		ifs.read((char*)&duration, sizeof(duration));
		ifs.read((char*)&sampleRate, sizeof(sampleRate));
		ifs.read((char*)&chunkCount, sizeof(chunkCount));
		std::vector<SoundChunk> baseSounds;
		for (int i = 0; i < chunkCount; i++)
		{
			baseSounds.push_back(readFromFileSoundChunk(ifs, sounds));
		}
		SoundChunk chunk = readFromFileSoundChunk(ifs, sounds);
		ifs.read((char*)&fade, sizeof(fade));
		return new Crossfade(duration, sampleRate, baseSounds, chunk, fade);
	}
	default:
		throw std::runtime_error("Unknown sound is trying to be loaded!");
	}
}

SoundChunk readFromFileSoundChunk(std::ifstream& ifs, const std::vector<const Sound*>& sounds)
{
	int soundIndex = 0;
	int startSample = 0;
	int endSample = 0;
	ifs.read((char*)&soundIndex, sizeof(soundIndex));
	ifs.read((char*)&startSample, sizeof(startSample));
	ifs.read((char*)&endSample, sizeof(endSample));
	return SoundChunk(*sounds[soundIndex], startSample, endSample);
}

Track readFromFileTrack(std::ifstream& ifs, const std::vector<const Sound*>& sounds)
{
	int sampleRate = 0;
	float duration = 0;
	unsigned trackSize = 0;
	ifs.read((char*)&sampleRate, sizeof(sampleRate));
	ifs.read((char*)&duration, sizeof(duration));
	ifs.read((char*)&trackSize, sizeof(trackSize));
	std::vector<SoundChunk> chunks;
	for (int i = 0; i < trackSize; i++)
	{
		chunks.push_back(readFromFileSoundChunk(ifs, sounds));
	}
	return Track(duration, sampleRate, chunks);
}