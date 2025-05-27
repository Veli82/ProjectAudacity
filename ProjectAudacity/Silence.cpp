#include "Silence.h"

Silence::Silence(double duration, int sampleRate)
    :Sound(duration, sampleRate)
{ }

float Silence::getSample(int index)
{
    validateIndex(index);

    return 0.0f;
}
