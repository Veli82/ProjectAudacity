#pragma once
#include "../Sound.h"

class Silence : public Sound
{
public:
	Silence(float duration, int sampleRate);

	float getSample(int index) const override;

	void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const override;
};

