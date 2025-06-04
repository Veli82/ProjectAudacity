#pragma once
#include <vector>

#include "../Sound/Sound.h"
#include "../Sound/SoundChunk.h"

class Track : public Sound
{
public:
	Track(int sampleRate);
	//Track(const Track& other);
	//Track& operator=(const Track& other) = delete;			//bukv defaultnite shte svurshat rabota

	void addSound(const Sound& sound);
	void addSound(const Sound& sound, int startSample);

	float getSample(int index) const override;

	//operator[] ?	mozhe da e polezen pri user interface-a

	//void writeToFile();
private:
	std::vector<SoundChunk> sounds;

	SoundChunk* findSound(int sampleIndexOnTrack, int& sampleIndexOnSound);
	const SoundChunk* findSound(int& sampleIndex) const;
};







//qka ideq, kato pravim effect za celiq track mozhe da mu podadem direktno sounds obekta (napravi mu const ref getter?)