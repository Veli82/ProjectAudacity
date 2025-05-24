#include "Sound.h"
#include <stdexcept>

Sound::Sound(double duration, unsigned sampleRate)
	:duration(duration), sampleRate(sampleRate), numOfSamples(duration * sampleRate)
{
	if (duration < 0)
	{
		throw std::runtime_error("Passed invalid Sound input");
	}
}
