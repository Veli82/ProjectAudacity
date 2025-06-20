#include "Tone.h"
#include <stdexcept>
#include <cmath>
#include "../readAndWriteUtils.h"

const float PI = 3.1415927f;


Tone::Tone(float duration, int sampleRate, int waveFrequency, float amplitude)
    :GeneratedSound(duration, sampleRate)
{
    setAmplitude(this->amplitude, amplitude);
    setWaveFrequency(this->waveFrequency,waveFrequency);
}

float Tone::getSample(int index) const
{
    validateIndex(index);
    return amplitude * std::sin(2.0f * PI * waveFrequency * ((float)index / sampleRate));
}

void Tone::save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
{
    SoundType type = SoundType::Tone;
    ofs.write((const char*)&type, sizeof(type));
    GeneratedSound::save(ofs, sounds);
    ofs.write((const char*)&waveFrequency, sizeof(waveFrequency));
    ofs.write((const char*)&amplitude, sizeof(amplitude));
}
