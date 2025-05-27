#pragma once
class Sound
{
public:
	//vish za vseki edin sound Big-4 --> trqbva da raboti dobre za da mozhe da ima comanda copy (na sound)
	Sound(double duration, int sampleRate);
	virtual float getSample(int index) = 0;
	//napravi oshte getteri?
protected:
	unsigned sampleRate;
	double duration;
	unsigned numOfSamples;

	void validateIndex(int index);
	void validateDurationAndSampleRate(double duration, int sampleRate);
};