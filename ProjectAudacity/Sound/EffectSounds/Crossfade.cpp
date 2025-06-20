#include "Crossfade.h"

#include <iostream>

Crossfade::Crossfade(float duration, int sampleRate, const std::vector<SoundChunk>& baseSounds, SoundChunk soundToCrossfade, bool fadeInNOut)
	:EffectSound(duration,sampleRate,baseSounds), soundToCrossfade(soundToCrossfade), fadeInNOut(fadeInNOut)
{
	if (soundToCrossfade.getNumOfSamples() != numOfSamples) throw std::runtime_error("Error when creating crossfade effect.");
}

float Crossfade::getSample(int index) const
{
    validateIndex(index);
    float s1 = getSampleFromBase(index);
    float s2 = soundToCrossfade.getSample(index);

    float coeff = (float)index / (numOfSamples - 1);
    float fade2 = coeff;
    if (fadeInNOut)
    {
        if (coeff < 0.5f) fade2 = coeff * 2.0f;
        else fade2 = (1.0f - coeff) * 2.0f;
    }
    float fade1 = 1 - fade2;
    return fade1 * s1 + fade2 * s2;
 }


float Crossfade::applyEffect(float sample) const
{
    return 0.0f;
}
     //for (size_t i = 0; i < length; ++i) {
     //   float t = static_cast<float>(i) / (length - 1);  // fade from 0 to 1
     //   result[i] = (1.0f - t) * soundA[i] + t * soundB[i];
     //   // Optional: clamp to ensure no overshoot due to rounding
     //   if (result[i] > 1.0f) result[i] = 1.0f;
     //   if (result[i] < -1.0f) result[i] = -1.0f;
