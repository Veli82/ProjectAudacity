#pragma once
#include "Sound.h"
#include <string>
#include <fstream>


class FileSound : public Sound
{
public:
	~FileSound();

const std::string& getFilePath() const;

protected:
	FileSound(const std::string& filePath);		//there shouldnt be objects of this class in the wild

	std::string filePath;
	std::ifstream reader;
	bool isStereo;
	unsigned blockAlign;
	unsigned bitsPerSample;		//bitDepth
	int firstSamplePos;
};

