#pragma once
class Sound
{
public:
	Sound(float duration, int sampleRate);
	virtual ~Sound() = default;

	virtual float getSample(int index) const = 0;

	//virtual Sound* clone() const = 0;
	//write to file
	//read from file
	
	int getSampleRate() const;
	float getDuration() const;
	int getNumOfSamples() const;
	
protected:
	int sampleRate;
	float duration;
	int numOfSamples;


	void validateIndex(int index) const;
	void validateDurationAndSampleRate(double duration, int sampleRate);
	};