#include "HighPassFilter.h"

HighPassFilter::HighPassFilter(double duration, unsigned sampleRate, const std::vector<SoundChunk>& baseSounds, int cutOffFreq)
    :EffectSound(duration, sampleRate, baseSounds), cutOffFreq(cutOffFreq)
{
    alpha = sampleRate / (sampleRate + 2 * 3.14 * cutOffFreq);
    lastInputSample = getSampleFromBase(0);
    lastOutputSample = lastInputSample;     //default values for the first applyEffect() call
}

float HighPassFilter::applyEffect(float sample)
{
    float result = alpha * (lastOutputSample + sample - lastInputSample);
    lastInputSample = sample;               //ideqta s keshiraneto predpolaga che shte se getvat sampulite POSLEDOVATELNO, koeto v nai chestiq sluchai e vqrno, but that's NOT the point!
    lastOutputSample = result;              // ne mi haresva
    return result;
}
