#pragma once
class Sound
{
public:
	Sound(double duration, unsigned sampleRate);
	virtual float getSample(int index) = 0;
	//napravi oshte getteri?
protected:
	unsigned sampleRate;
	double duration;
	unsigned numOfSamples;
};

