#include "WhiteNoise.h"
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include "../readAndWriteUtils.h"



WhiteNoise::WhiteNoise(float duration, int sampleRate, float amplitude)
    :GeneratedSound(duration, sampleRate)
{
    setAmplitude(this->amplitude, amplitude);
}

float WhiteNoise::getSample(int index) const
{
    validateIndex(index);
    float rand = float(std::rand()) / RAND_MAX;
    return amplitude * (2.0f * rand - 1.0f);
}

void WhiteNoise::save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
{
    SoundType type = SoundType::WhiteNoise;
    ofs.write((const char*)&type, sizeof(type));
    GeneratedSound::save(ofs, sounds);
    ofs.write((const char*)&amplitude, sizeof(amplitude));

}

