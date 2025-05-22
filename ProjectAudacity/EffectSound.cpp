#include "EffectSound.h"

EffectSound::EffectSound(double duration, double sampleRate, const std::vector<SoundChunk>& baseSounds)
	:Sound(duration, sampleRate), baseSounds(baseSounds)
{ }

float EffectSound::getSample(int index) const
{//make input validation!
	return applyEffect(getSampleFromBase(index));
}

float EffectSound::getSampleFromBase(int index) const
{
	{
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
}
