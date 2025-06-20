#pragma once
#include <fstream>
#include <vector>

class Sound
{
public:
	Sound(float duration, int sampleRate);
	virtual ~Sound() = default;

	virtual float getSample(int index) const = 0;

	virtual void save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const = 0;	//save
	
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