#pragma once
#include "../Sound.h"
#include <string>
#include <fstream>


class FileSound : public Sound
{
public:
	FileSound(const std::string& filePath);
	~FileSound();
	//i think mby i shoud rewrite copy c-tor and op=? (because ifstream object cant be copied implicitly)

const std::string& getFilePath() const;

protected:
	std::string filePath;
	std::ifstream* reader;
	bool isStereo;
	unsigned blockAlign;
	unsigned bitsPerSample;		//bitDepth
	int firstSamplePos;
};

