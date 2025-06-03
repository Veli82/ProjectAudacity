#pragma once
#include "EffectSound.h"

class Amplify : public EffectSound
{
public:
	Amplify(float duration, int sampleRate, const std::vector<SoundChunk>& baseSounds, float decibels);
protected:
	float applyEffect(float sample) const override;

	float ampDecibels;
	float ampFactor;
};

