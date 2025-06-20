#pragma once
#include "../Sound.h"

class GeneratedSound : public Sound
{
public:
	GeneratedSound(float duration, int sampleRate);

	void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const override;
protected:
	void setAmplitude(float& destination, float amplitude);
	void setWaveFrequency(int& destination, int frequency);
};