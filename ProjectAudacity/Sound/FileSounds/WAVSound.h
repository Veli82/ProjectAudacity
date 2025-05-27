#pragma once
#include "FileSound.h"

class WAVSound : public FileSound
{
public:
	WAVSound(const std::string& filePath);

	float getSample(int index) override;
protected:

};

