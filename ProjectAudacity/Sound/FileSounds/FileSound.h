#pragma once
#include "../Sound.h"
#include <string>
#include <fstream>


class FileSound : public Sound
{
public:
	FileSound(const std::string& filePath);
	FileSound(const FileSound& other);
	FileSound& operator=(const FileSound& other) = delete;

	const std::string& getFilePath() const;
	bool isItStereo() const;
	unsigned getBlockAlign() const;
	unsigned getBitsPerSample() const;

	void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const override;

protected:
	std::string filePath;
	mutable std::ifstream reader;
	bool isStereo;
	unsigned blockAlign;
	unsigned bitsPerSample;		//bitDepth
	int firstSamplePos;
};

