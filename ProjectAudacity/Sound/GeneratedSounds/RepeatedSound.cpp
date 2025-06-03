#include "RepeatedSound.h"

RepeatedSound::RepeatedSound(int sampleRate, Sound* soundToRepeat, int timesRepeated)
    :GeneratedSound(0, sampleRate)
{
    //make validation!
    this->soundToRepeat = soundToRepeat;
    this->timesRepeated = timesRepeated;
    //po skoro izchisli numOfSamples * timesRepeated i ot tam izchisli duration za da e po tochno! (pravih integer umnozhenie, persisiona na duration e vse taq)
    duration = soundToRepeat->getDuration() * timesRepeated; //make getter for duration!
    numOfSamples;// = ... TODO

}

float RepeatedSound::getSample(int index) const
{ 
    validateIndex(index);
    int sampleToGet = index % soundToRepeat->getNumOfSamples();
    return soundToRepeat->getSample(sampleToGet);
}
