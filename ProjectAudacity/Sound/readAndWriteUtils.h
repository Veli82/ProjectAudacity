#pragma once
#include <vector>
#include <fstream>
#include "Sound.h"
class SoundChunk;
class Track;

enum class SoundType
{
	WAVSound,
	AIFFSound,
	Silence,
	Tone,
	WhiteNoise,
	Chirp,
	RepeatedSound,
	Amplify,
	HighPassFilter,
	Crossfade
};


int getIndexOfSound(const std::vector<const Sound*>& sounds, const Sound* sound);

Sound* readFromFileSoundFactory(std::ifstream& ifs, const std::vector<const Sound*>& sounds);

SoundChunk readFromFileSoundChunk(std::ifstream& ifs, const std::vector<const Sound*>& sounds);

Track readFromFileTrack(std::ifstream& ifs, const std::vector<const Sound*>& sounds);