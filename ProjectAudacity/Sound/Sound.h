#pragma once
class Sound
{
public:
	//vish za vseki edin sound Big-4 --> trqbva da raboti dobre za da mozhe da ima comanda copy (na sound)
	//mozhe bi za vseki sound trqbva da ima copy constructor, no nqma smisul da ima operator= (smis bukv za kakvo mi e?)
	Sound(float duration, int sampleRate);

	virtual float getSample(int index) const = 0;
	
	unsigned getSampleRate() const;
	float getDuration() const;
	unsigned getNumOfSamples() const;
	
protected:
	unsigned sampleRate;		//should it be unsigned or int?
	float duration;
	unsigned numOfSamples;


	void validateIndex(int index) const;
	void validateDurationAndSampleRate(double duration, int sampleRate);
	//napravi 2 protected settera?
};