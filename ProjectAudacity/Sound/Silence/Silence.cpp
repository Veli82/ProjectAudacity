#include "Silence.h"

Silence::Silence(float duration, int sampleRate)
    :Sound(duration, sampleRate)
{ }

float Silence::getSample(int index) const
{
    validateIndex(index);

    return 0.0f;
}
