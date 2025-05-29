#include "Tone.h"
#include <stdexcept>
#include <cmath>

const float PI = 3.1415927f;


Tone::Tone(float duration, int sampleRate, int waveFrequency, float amplitude)
    :GeneratedSound(duration, sampleRate)
{
    setAmplitude(this->amplitude, amplitude);
    setWaveFrequency(this->waveFrequency,waveFrequency);
}

float Tone::getSample(int index) const
{
    return amplitude * std::sin(2.0f * PI * waveFrequency * ((float)index / sampleRate));
}
