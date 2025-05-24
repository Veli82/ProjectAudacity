#pragma once
#include "EffectSound.h"

class Amplify : public EffectSound
{
public:
	Amplify(double duration, unsigned sampleRate, const std::vector<SoundChunk>& baseSounds, float decibels);
protected:
	float applyEffect(float sample) override;

	const float ampDecibels;
	const float ampFactor;

};

