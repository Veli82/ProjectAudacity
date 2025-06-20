#include "Amplify.h"
#include "../../Utils.h"

#include <cmath>

Amplify::Amplify(float duration, int sampleRate, const std::vector<SoundChunk>& baseSounds, float decibels)
    : EffectSound(duration, sampleRate, baseSounds), ampDecibels(decibels), ampFactor(std::pow(10, ampDecibels / 20))
{
    if (decibels < -30 || decibels > 30) throw std::runtime_error("Invalid input for decibles when making Amplify");
}

float Amplify::applyEffect(float sample) const
{
    //applies clipping
    return clamp(sample * ampFactor, -1.0f, 1.0f);
}
