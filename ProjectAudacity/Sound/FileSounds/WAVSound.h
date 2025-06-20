#pragma once
#include "FileSound.h"

class WAVSound : public FileSound
{
public:
	WAVSound(const std::string& filePath);

	void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const override;

	float getSample(int index) const override;

};

