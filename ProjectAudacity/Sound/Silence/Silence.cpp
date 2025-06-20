#include "Silence.h"
#include "../readAndWriteUtils.h"

Silence::Silence(float duration, int sampleRate)
    :Sound(duration, sampleRate)
{ }

float Silence::getSample(int index) const
{
    validateIndex(index);

    return 0.0f;
}

void Silence::save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
{
    SoundType type = SoundType::Silence;
    ofs.write((const char*)&type, sizeof(type));
    Sound::save(ofs, sounds);
}
