#include "Amplify.h"
#include "Utils.h"

#include <cmath>

Amplify::Amplify(double duration, int sampleRate, const std::vector<SoundChunk>& baseSounds, float decibels)
    : EffectSound(duration, sampleRate, baseSounds), ampDecibels(decibels), ampFactor(std::pow(10, ampDecibels / 20))
{
}

float Amplify::applyEffect(float sample)
{
    //applies clipping
    return clamp(sample * ampFactor, -1.0f, 1.0f);
}
