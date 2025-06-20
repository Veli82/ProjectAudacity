#pragma once
#include <vector>
#include <fstream>

#include "../Track/Track.h"
#include "../Sound/Sound.h"
#include "../Sound/SoundChunk.h"

class Project
{
public:
	Project(int sampleRate, int numOfTracks);
	//Project(std::ifstream& ifs);	//for loading a project
	Project(const Project& other) = delete;
	Project& operator=(const Project& other) = delete;
	~Project();

	void addSoundToTrack(const Sound* sound, int trackIndex, int startSample);
	void appendSoundToTrack(const Sound* sound, int trackIndex);

	void exportProject(const char* filename) const;
	//void saveProject(string) const;		//for saving a project
	

	void runUI();

private:
	std::vector<Track> tracks;
	std::vector<const Sound*> allSounds;
	int sampleRate;


	int askUserForTrackIndex() const;
	const SoundChunk& askUserToChooseASound(int trackIndex = -1) const;
	bool askUserToAppendOrNot() const;
	int askUserForSampleOnTrack() const;
	void checkSampleRate(const Sound* sound);
	void addSoundToProjectUI(const Sound* sound);
	void exportUI() const;

	Sound* createFileSoundUI() const;
	Sound* createGeneratedSoundUI() const;
	void createAndApplyEffectUI();
	Sound* createEffectFromBaseSoundUI(std::vector<SoundChunk> baseSounds, float duration) const;

};