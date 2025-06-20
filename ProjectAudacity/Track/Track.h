#pragma once
#include <vector>

#include "../Sound/Sound.h"
#include "../Sound/SoundChunk.h"

class Track : public Sound
{
public:
	Track(int sampleRate);
	Track(float duration, int sampleRate, const std::vector<SoundChunk>& sounds);	//added last minute for reading from file

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
	const std::vector<SoundChunk>& getSoundsArr() const;
	unsigned getChunksCount() const;

	void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const override;

private:
	std::vector<SoundChunk> sounds;

	SoundChunk* findSound(int sampleIndexOnTrack, int& sampleIndexOnSound);
	const SoundChunk* findSound(int sampleIndexOnTrack, int& sampleIndexOnSound) const;
	void removeEmptyChunks();
};