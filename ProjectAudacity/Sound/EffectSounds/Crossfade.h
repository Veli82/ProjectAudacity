#pragma once
#include "../EffectSounds/EffectSound.h"

class Crossfade : public EffectSound
{
public:
	Crossfade(float duration, int sampleRate, const std::vector<SoundChunk>& baseSounds, SoundChunk soundToCrossfade, bool fadeInNOut);

	void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const override;

	float getSample(int index) const override;
protected:
	SoundChunk soundToCrossfade;
	bool fadeInNOut;

	float applyEffect(float sample) const override;
};

