#pragma once
class Sound
{
public:
	Sound(float duration, int sampleRate);
	virtual ~Sound() = default;

	virtual float getSample(int index) const = 0;

	//write to file
	//read from file
	
	int getSampleRate() const;
	float getDuration() const;
	int getNumOfSamples() const;

	
protected:
	int sampleRate;
	float duration;
	int numOfSamples;

	void setDuration(float durarion);
	void setNumOfSamples(int numOfSamples);
	void setNumOfSamplesAndDur(int numOfSamples);
	void setSampleRate(int sampleRate);

	void validateIndex(int index) const;
	};