#pragma once
#include <vector>

#include "../Track/Track.h"
#include "../Sound/Sound.h"
#include "../Sound/SoundChunk.h"

class Project
{
public:

	Sound* createSound();	//creates the object, adds it in allSounds and calls addSound() to the correct track, passing a reference to the sound it just created.
	void exportProject();

private:
	std::vector<Track> tracks;
	std::vector<Sound*> allSounds;
};

