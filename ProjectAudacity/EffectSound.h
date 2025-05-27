#pragma once
#include "Sound.h"
#include "SoundChunk.h"

#include <vector>

class EffectSound : public Sound
{
public:
	EffectSound(double duration, int sampleRate,const std::vector<SoundChunk>& baseSounds);
	float getSample(int index) override;
protected:
	const std::vector<SoundChunk> baseSounds;

	float getSampleFromBase(int index) const;
	virtual float applyEffect(float sample) = 0;
};

