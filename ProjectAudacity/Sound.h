#pragma once
class Sound
{
public:
	Sound(double duration, double sampleRate);
	virtual float getSample(int index) const = 0;
protected:
	const float sampleRate;
	const double duration;
	const int numOfSamples;
};

