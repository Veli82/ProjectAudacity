#pragma once
class Sound
{
public:
	Sound(double duration, double sampleRate);
	virtual float getSample(int index) = 0;
protected:
	float sampleRate;
	double duration;
	int numOfSamples;
};

