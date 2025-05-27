#include "Sound.h"
#include <stdexcept>

Sound::Sound(double duration, int sampleRate)
	:duration(duration), sampleRate(sampleRate), numOfSamples(duration * sampleRate)
{
	validateDurationAndSampleRate(duration, sampleRate);
}

void Sound::validateIndex(int index)
{
	if (index < 0 || index >= numOfSamples)
	{
		throw std::runtime_error("Invalid index passed when trying to get a sound sample!");
	}
}

void Sound::validateDurationAndSampleRate(double duration, int sampleRate)
{
	if (duration < 0 || sampleRate <= 0 )	//maybe improve in the future?
	{
		throw std::runtime_error("Passed invalid Sound input");
	}
}
