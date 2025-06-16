#pragma once
#include <vector>

#include "../Sound/Sound.h"
#include "../Sound/SoundChunk.h"

class Track : public Sound
{
public:
	Track(int sampleRate);

	Track& operator=(const Track& other) = delete;

	void addSound(const Sound& sound);						//append on end
	void addSound(const Sound& sound, SoundChunk& dest);	//put on top of another SoundChunk
	void addSound(const Sound& sound, int startSample);		//insert at specific place

	float getSample(int index) const override;

	void writeToFile(const char* filename) const;
	void writeToFileTest(float* arr) const;

	SoundChunk& operator[](int index);
	const SoundChunk& operator[](int index) const;

	std::vector<SoundChunk> getChunks(int startSample, int endSample) const;
private:
	std::vector<SoundChunk> sounds;

	SoundChunk* findSound(int sampleIndexOnTrack, int& sampleIndexOnSound);
	const SoundChunk* findSound(int sampleIndexOnTrack, int& sampleIndexOnSound) const;
	void removeEmptyChunks();
	void writeWavHeader(std::ofstream& ofs) const;
};







//qka ideq, kato pravim effect za celiq track mozhe da mu podadem direktno sounds obekta (napravi mu const ref getter?)