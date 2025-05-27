#pragma once
#include "FileSound.h"

class AIFFSound : public FileSound
{
public:
	AIFFSound(const std::string& filePath);

	float getSample(int index) override;
};

