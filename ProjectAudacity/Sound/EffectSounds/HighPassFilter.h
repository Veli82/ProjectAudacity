#pragma once
#include "EffectSound.h"

class HighPassFilter : public EffectSound
{
public:
	//HighPassFilter(float duration, int sampleRate, const std::vector<SoundChunk>& baseSounds, int cutOffFreq);

	//void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const override;
protected:
	//float applyEffect(float sample) const override;	//v momenta ne raboti

	//int cutOffFreq;
	//float alpha;

	//cache
	//float lastInputSample;
	//float lastOutputSample;
};

