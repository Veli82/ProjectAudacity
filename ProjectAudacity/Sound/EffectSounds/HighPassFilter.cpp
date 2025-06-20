#include "HighPassFilter.h"

//const float PI = 3.1415927f;
//
//HighPassFilter::HighPassFilter(float duration, int sampleRate, const std::vector<SoundChunk>& baseSounds, int cutOffFreq)
//    :EffectSound(duration, sampleRate, baseSounds), cutOffFreq(cutOffFreq)
//{
//    if (cutOffFreq <= 0 || cutOffFreq > 20000) throw std::runtime_error("Invalid input on cutoff frequency in high pass filter.");
//    alpha = (float)sampleRate / (sampleRate + 2 * PI * cutOffFreq);
//    lastInputSample = getSampleFromBase(0);
//    lastOutputSample = lastInputSample;     //default values for the first applyEffect() call
//}
//
//float HighPassFilter::applyEffect(float sample) const
//{
//    float result = alpha * (lastOutputSample + sample - lastInputSample);
//    //lastInputSample = sample;               //ideqta s keshiraneto predpolaga che shte se getvat sampulite POSLEDOVATELNO, koeto v nai chestiq sluchai e vqrno, but that's NOT the point!
//    //lastOutputSample = result;              // ne mi haresva
//    return result;
//}
