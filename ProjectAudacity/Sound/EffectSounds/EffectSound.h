#pragma once
#include "../Sound.h"
#include "../SoundChunk.h"

#include <vector>

class EffectSound : public Sound
{
public:
	EffectSound(float duration, int sampleRate,const std::vector<SoundChunk>& baseSounds);
	float getSample(int index) const override;
protected:
	const std::vector<SoundChunk> baseSounds;	//should this actually be const?

	float getSampleFromBase(int index) const;
	virtual float applyEffect(float sample) const = 0;
};

