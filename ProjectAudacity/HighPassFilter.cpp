#include "HighPassFilter.h"

HighPassFilter::HighPassFilter(double duration, double sampleRate, const std::vector<SoundChunk>& baseSounds, int cutOffFreq)
    :EffectSound(duration, sampleRate, baseSounds), cutOffFreq(cutOffFreq)
{
    alpha = sampleRate / (sampleRate + 2 * 3.14 * cutOffFreq);
    lastInputSample = getSampleFromBase(0);
    lastOutputSample = lastInputSample;     //default values for the first applyEffect() call
}

float HighPassFilter::applyEffect(float sample)
{
    float result = alpha * (lastOutputSample + sample - lastInputSample);
    lastInputSample = sample;
    lastOutputSample = result;
    return result;
}
