#include "WhiteNoise.h"
#include <stdexcept>
#include <cstdlib>
#include <ctime>



WhiteNoise::WhiteNoise(double duration, int sampleRate, float amplitude)
    :GeneratedSound(duration, sampleRate)
{
    setAmplitude(this->amplitude, amplitude);
}

float WhiteNoise::getSample(int index)
{
    float rand = float(std::rand()) / RAND_MAX;
    return 2.0f * rand - 1.0f;
}

