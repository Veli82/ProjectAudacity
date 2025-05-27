#include "EffectSound.h"

EffectSound::EffectSound(double duration, int sampleRate, const std::vector<SoundChunk>& baseSounds)
	:Sound(duration, sampleRate), baseSounds(baseSounds)
{ }

float EffectSound::getSample(int index)
{
	validateIndex(index);
	return applyEffect(getSampleFromBase(index));
}

float EffectSound::getSampleFromBase(int index) const
{
	//index should always be valid

	for (int i = 0; i < baseSounds.size(); i++)
	{
		if (index < baseSounds[i].numofSamples)
		{
			return baseSounds[i].sound->getSample(baseSounds[i].startSample + index);
		}
		else
		{
			index -= baseSounds[i].numofSamples;
		}
	}
}
