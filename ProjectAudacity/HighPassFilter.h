#pragma once
#include "EffectSound.h"

class HighPassFilter : public EffectSound
{
public:
	HighPassFilter(double duration, double sampleRate, const std::vector<SoundChunk>& baseSounds, int cutOffFreq);
protected:
	float applyEffect(float sample) override;

	const int cutOffFreq;
	float alpha;

	//cache
	float lastInputSample;
	float lastOutputSample;
};

