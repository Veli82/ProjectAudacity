#pragma once
#include "Sound.h"
#include "SoundChunk.h"

#include <vector>

class EffectSound : public Sound
{
public:
	EffectSound(double duration, unsigned sampleRate,const std::vector<SoundChunk>& baseSounds);
	float getSample(int index) override;
protected:
	float getSampleFromBase(int index) const;
	virtual float applyEffect(float sample) = 0;

	const std::vector<SoundChunk> baseSounds;
};

