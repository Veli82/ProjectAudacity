#include "GeneratedSound.h"
#include <stdexcept>

GeneratedSound::GeneratedSound(float duration, int sampleRate)
	:Sound(duration, sampleRate)
{ }

void GeneratedSound::setAmplitude(float& destination, float amplitude)
{
    if (amplitude < 0.0f || amplitude > 1.0f)
    {
        throw std::runtime_error("Invalid Input when generating a sound!");
    }
    destination = amplitude;
}

void GeneratedSound::setWaveFrequency(int& destination, int frequency)
{
    if (frequency <= 0 || frequency > 20000)
    {
        throw std::runtime_error("Invalid Input when generating a sound!");
    }
    destination = frequency;
}

