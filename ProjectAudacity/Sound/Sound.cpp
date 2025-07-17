#include "Sound.h"
#include <stdexcept>

Sound::Sound(float duration, int sampleRate)
{
	setSampleRate(sampleRate);
	setDuration(duration);
	setNumOfSamples(duration * sampleRate);
}

void Sound::save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
{
	ofs.write((const char*)&duration, sizeof(duration));
	ofs.write((const char*)&sampleRate, sizeof(sampleRate));
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

void Sound::setDuration(float duration)
{
	if (duration < 0 || duration > INT_MAX / (float)sampleRate)
	{
		throw std::runtime_error("Passed invalid Sound input");
	}
	this->duration = duration;
	this->numOfSamples = duration * sampleRate;
}

void Sound::setNumOfSamples(int numOfSamples)
{
	float dur = (float)numOfSamples / sampleRate;
	if (numOfSamples < 0 || dur < 0)
	{
		throw std::runtime_error("Passed invalid Sound input. Probably sound is too long");
	}
	this->numOfSamples = numOfSamples;
	this->duration = dur;
}


void Sound::setSampleRate(int sampleRate)
{
	//not quite sure what behaviour do I want for this
	if (sampleRate <= 0)
	{
		throw std::runtime_error("Passed invalid Sound input");
	}
	this->sampleRate = sampleRate;
}

void Sound::validateIndex(int index) const
{
	if (index < 0 || index >= numOfSamples)
	{
		throw std::runtime_error("Invalid index passed when trying to get a sound sample!");
	}
}