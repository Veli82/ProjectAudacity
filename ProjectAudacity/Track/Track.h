#pragma once
#include <vector>

#include "../Sound/Sound.h"
#include "../Sound/SoundChunk.h"

class Track
{
public:
	Track();
	Track(const Track& other);
	Track& operator=(const Track& other) = delete;
	~Track();

	void addSound(const Sound& sound);

	float getSample(int index) const;

	//operator[] ?


	int getSampleRate() const;

	//void writeToFile();
private:
	std::vector<SoundChunk> sounds;
	int sampleRate;
};

