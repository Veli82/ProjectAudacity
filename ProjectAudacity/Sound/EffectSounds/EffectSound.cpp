#include "EffectSound.h"

EffectSound::EffectSound(float duration, int sampleRate, const std::vector<SoundChunk>& baseSounds)
	:Sound(duration, sampleRate), baseSounds(baseSounds)
{ }

float EffectSound::getSample(int index) const
{
	validateIndex(index);
	return applyEffect(getSampleFromBase(index));
}

void EffectSound::save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
{
	Sound::save(ofs, sounds);
	int chunkCount = baseSounds.size();
	ofs.write((const char*)&chunkCount, sizeof(chunkCount));
	for (const SoundChunk& chunk : baseSounds)
	{
		chunk.save(ofs, sounds);
	}
}

float EffectSound::getSampleFromBase(int index) const
{
	//index should always be valid

	for (int i = 0; i < baseSounds.size(); i++)
	{
		int chunkSize = baseSounds[i].getNumOfSamples();
		if (index < chunkSize)
		{
			return baseSounds[i].getSample(index);
		}
		else
		{
			index -= chunkSize;
		}
	}
}
