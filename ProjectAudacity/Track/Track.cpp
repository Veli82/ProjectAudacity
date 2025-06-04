#include "Track.h"

Track::Track(int sampleRate)
    :Sound(0, sampleRate)
{
}

void Track::addSound(const Sound& sound)
{
	SoundChunk chunk(sound);
	sounds.push_back(chunk);
}

//oshte edin addSound(const Sound& sound) koito prosto da appendva!

//i oshte edin koito da priema direktno sound chunk(s?) i da go zamestva bez da go resizeva (samo da prenasochva pointera)


void Track::addSound(const Sound& sound, int startSampleOnTrack)		//trqbva da vnimavam da ne se dobavi track v track;
{
	//startSampleOnTrack needs to be validated, but maybe it should be outside of this function and not here (so just put assert here just in case)
	int endSampleOnTrack = startSampleOnTrack + sound.getNumOfSamples();
	int startSampleOnSound;
	int endSampleOnSound;
	SoundChunk* firstChunk = findSound(startSampleOnTrack, startSampleOnSound);
	SoundChunk* lastChunk = findSound(endSampleOnTrack, endSampleOnSound);

	//could be made with less code in the if statement
	if (firstChunk && lastChunk)
	{
		//promenqme nachalo i krai na susednite chunkove, iztrivame tezi deto ne ni trqbvat, insertvame kudeto trqbva i sme top
		firstChunk->setEnd(startSampleOnSound);
		lastChunk->setStart(endSampleOnSound);
		SoundChunk chunk(sound);
		std::vector<SoundChunk>::iterator firstChunkPos = std::find(sounds.begin(), sounds.end(), firstChunk);
		std::vector<SoundChunk>::iterator lastChunkPos = std::find(sounds.begin(), sounds.end(), lastChunk);
		sounds.erase(firstChunkPos + 1, lastChunkPos);
		sounds.insert(firstChunkPos + 1, chunk);
		//mazalooo ama mai raboti
	}
	else if (firstChunk && !lastChunk)
	{
		//promenqme krai na purviq chunk, iztrivame do kraq, appendvame sounda i dobavqme kum tracka (endaSampleOnTrack - Track::numOfSamples)
		firstChunk->setEnd(startSampleOnSound);
		SoundChunk chunk(sound);
		std::vector<SoundChunk>::iterator firstChunkPos = std::find(sounds.begin(), sounds.end(), firstChunk);
		sounds.erase(firstChunkPos + 1, sounds.end());
		sounds.push_back(sound);
		numOfSamples = endSampleOnTrack;
		duration = (float)numOfSamples / sampleRate;

	}
	else if (!firstChunk && !lastChunk)		//not sure if i shoud even implement this case here?	(maybe the silence should get handled outside)
		//da da opredeleno ne go implementirai tuk. poroverakata lesno shte stane sus startSampleOnTrack > track.numOfSamples
		//otvun silently shte se vika addSound(silence) /append versiqta/ koqto shte slaga sound dulug kolkoto trqbva za da stignem do startSampleOnTrack
		//aide be mozhe i da stanee aaa
	{
		// add silence until startSampleOnTrack(how?) append sound i dobavqme kum traka (sound.numOfSamples + silence.numOfSamples)
	}
	else
	{
		throw std::runtime_error("Invalid data in Track::addSound(), we should not be here");
	}

}

float Track::getSample(int index) const
{
    validateIndex(index);
	return findSound(index)->getSample(index);
}

SoundChunk* Track::findSound(int sampleIndexOnTrack, int& sampleIndexOnSound)
{
	sampleIndexOnSound = sampleIndexOnTrack;
	for (int i = 0; i < sounds.size(); i++)
	{
		int chunkSize = sounds[i].getNumOfSamples();
		if (sampleIndexOnSound < chunkSize)
		{
			return &sounds[i];
		}
		else
		{
			sampleIndexOnSound -= chunkSize;
		}
	}
	return nullptr;	//case where sampleIndexOnTrack > track.numOfSamples
}

const SoundChunk* Track::findSound(int& sampleIndexOnSound) const
{
	for (int i = 0; i < sounds.size(); i++)
	{
		int chunkSize = sounds[i].getNumOfSamples();
		if (sampleIndexOnSound < chunkSize)
		{
			return &sounds[i];
		}
		else
		{
			sampleIndexOnSound -= chunkSize;
		}
	}
	return nullptr;
}





