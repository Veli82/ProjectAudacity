#include "RepeatedSound.h"

RepeatedSound::RepeatedSound(int sampleRate,const SoundChunk& soundToRepeat, int timesRepeated)
    :GeneratedSound(0, sampleRate), soundToRepeat(soundToRepeat)
{
    if (timesRepeated <= 0) throw std::runtime_error("invalid input on number of times repeated.");
    this->timesRepeated = timesRepeated;
    numOfSamples = soundToRepeat.getNumOfSamples() * timesRepeated;
    duration = (float)numOfSamples / sampleRate;
}

float RepeatedSound::getSample(int index) const
{ 
    validateIndex(index);
    int sampleToGet = index % soundToRepeat.getNumOfSamples();
    return soundToRepeat.getSample(sampleToGet);
}




//sega se setih za problem che toq Sound* shte sochi kum celiq zvuk, ne samo soundchunka koito e v traka
//mozhe da go implementirash, mozhe i da go ostavish ne e that big of a deal