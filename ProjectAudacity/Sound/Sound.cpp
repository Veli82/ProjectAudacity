#include "Sound.h"
#include <stdexcept>

Sound::Sound(float duration, int sampleRate)
	:duration(duration), sampleRate(sampleRate), numOfSamples(duration * sampleRate)
{
	validateDurationAndSampleRate(duration, sampleRate);
}

int Sound::getSampleRate() const
{
	return sampleRate;
}

float Sound::getDuration() const
{
	return duration;
}

int Sound::getNumOfSamples() const
{
	return numOfSamples;
}

void Sound::validateIndex(int index) const
{
	if (index < 0 || index >= numOfSamples)
	{
		throw std::runtime_error("Invalid index passed when trying to get a sound sample!");
	}
}

void Sound::validateDurationAndSampleRate(double duration, int sampleRate)
{
	if (duration < 0 || sampleRate <= 0 || numOfSamples > INT_MAX)
	{
		throw std::runtime_error("Passed invalid Sound input");
	}
}
