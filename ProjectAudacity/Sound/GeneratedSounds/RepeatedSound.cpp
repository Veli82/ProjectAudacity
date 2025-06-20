#include "RepeatedSound.h"
#include "../readAndWriteUtils.h"

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

void RepeatedSound::save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
{
    SoundType type = SoundType::RepeatedSound;
    ofs.write((const char*)&type, sizeof(type));
    ofs.write((const char*)&sampleRate, sizeof(sampleRate));
    soundToRepeat.save(ofs, sounds);
    ofs.write((const char*)&timesRepeated, sizeof(timesRepeated));
}