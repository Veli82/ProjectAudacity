#include "Sound.h"
#include <stdexcept>

Sound::Sound(double duration, double sampleRate)
{
	if (duration < 0)
	{
		throw std::runtime_error("Passed invalid Sound input");
	}
	this->duration = duration;
	this->sampleRate = sampleRate;
	numOfSamples = duration * sampleRate;	//implicit conversion from double to int	/shoud not be a problem
}
