#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

#include "../Sound/FileSounds/WAVSound.h"
#include "../Sound/FileSounds/AIFFSound.h"
#include "../Sound/FileSounds/WAVHeader.h"
#include "../Sound/Silence/Silence.h"
#include "../Sound/EffectSounds/Amplify.h"
#include "../Sound/EffectSounds/HighPassFilter.h"
#include "../Sound/EffectSounds/Crossfade.h"
#include "../Sound/GeneratedSounds/Tone.h"
#include "../Sound/GeneratedSounds/WhiteNoise.h"
#include "../Sound/GeneratedSounds/Chirp.h"
#include "../Sound/GeneratedSounds/RepeatedSound.h"
#include "../Sound/readAndWriteUtils.h"

#include "Project.h"

//settings for exporting the track, implementation for other values could be made in the future.
static short numOfChannels = 1;
static short bitsPerSample = 16;


//the longest sound the program can handle (with a standart sample rate of 44.1kHz) is 48695 seconds ~ 811 minutes ~ 13.5 hours
//on my machine it took 11 minutes to export

Project::Project(int sampleRate, int numOfTracks)
{
	if (sampleRate <= 0) throw std::runtime_error("Invalid Input!");
	this->sampleRate = sampleRate;
	for (int i = 0; i < numOfTracks; i++)
	{
		tracks.push_back(Track(sampleRate));
	}
}

Project::Project(std::ifstream& ifs)
{
	ifs.read((char*)&sampleRate, sizeof(sampleRate));
	unsigned allSoundsSize;
	ifs.read((char*)&allSoundsSize, sizeof(allSoundsSize));
	try
	{
		for (int i = 0; i < allSoundsSize; i++)
		{
			Sound* sound = readFromFileSoundFactory(ifs, allSounds);
			allSounds.push_back(sound);
		}
		unsigned tracksCount;
		ifs.read((char*)&tracksCount, sizeof(tracksCount));
		for (int i = 0; i < tracksCount; i++)
		{
			Track track = readFromFileTrack(ifs, allSounds);
			tracks.push_back(track);
		}
	}
	catch (...)
	{
		for (int i = 0; i < allSounds.size(); i++)
		{
			delete allSounds[i];
			throw;
		}
	}
}

Project::~Project()
{
	int size = allSounds.size();
	for (int i = 0; i < size; i++)
	{
		delete allSounds[i];
	}
}

void Project::addSoundToTrack(const Sound* sound, int trackIndex, int startSampleOnTrack)
{
	Track& track = tracks[trackIndex];
	int trackNumOfSamples = track.getNumOfSamples();

	if (startSampleOnTrack < trackNumOfSamples)	//if the sound starts inside the track
	{
		float fadeDur;
		Sound* crFade;
		if (startSampleOnTrack - 1 > INT_MAX - sound->getNumOfSamples()) throw std::runtime_error("Track's max size is reached!");
		int endSampleOnTrack = startSampleOnTrack + sound->getNumOfSamples() - 1;
		if (endSampleOnTrack < trackNumOfSamples)	//if the sound ends inside the track
		{
			std::vector<SoundChunk> baseSounds = track.getChunks(startSampleOnTrack, endSampleOnTrack);
			fadeDur = sound->getNumOfSamples() / (float)sampleRate;
			crFade = new Crossfade(fadeDur, sampleRate, baseSounds, *sound, true);
		}
		else	//the sound does not end inside the track
		{
			std::vector<SoundChunk> baseSounds = track.getChunks(startSampleOnTrack, trackNumOfSamples - 1);
			fadeDur = (trackNumOfSamples - startSampleOnTrack) / (float)sampleRate;
			SoundChunk soundCh(*sound);
			soundCh.setEnd(trackNumOfSamples - startSampleOnTrack);
			track.addSound(*sound,startSampleOnTrack);
			crFade = new Crossfade(fadeDur, sampleRate, baseSounds, soundCh, false);
		}

		try
		{
			track.addSound(*crFade, startSampleOnTrack);
			allSounds.push_back(sound);
			allSounds.push_back(crFade);
		}
		catch (...)
		{
			delete crFade;
			throw;
		}

	}
	else	////the sound does not start inside the track
	{
		float silenceDur = (startSampleOnTrack - trackNumOfSamples) / (float)sampleRate;
		Sound* silence = new Silence(silenceDur, sampleRate);
		try
		{
			track.addSound(*silence);
			track.addSound(*sound);
			allSounds.push_back(sound);
			allSounds.push_back(silence);
		}
		catch (...)
		{
			delete silence;
			throw;
		}

	}
}

void Project::appendSoundToTrack(const Sound* sound, int trackIndex)
{
	tracks[trackIndex].addSound(*sound);
	allSounds.push_back(sound);
}

void Project::exportProject(const char* filename) const
{
	std::ofstream ofs(filename, std::ios::binary);
	if (!ofs.is_open())
	{
		throw std::runtime_error("could not open file for writing");
	}

	//get numOfSamples of the longest track
	int maxNumOfSamples = tracks[0].getNumOfSamples();
	for (int i = 1; i < tracks.size(); i++)
	{
		if (tracks[i].getNumOfSamples() > maxNumOfSamples)
		{
			maxNumOfSamples = tracks[i].getNumOfSamples();
		}
	}

	writeWavHeader(ofs, maxNumOfSamples, sampleRate, numOfChannels, bitsPerSample);


	//for normalization
	float maxAmplSample = 0;
	for (int i = 0; i < maxNumOfSamples; i++)
	{
		float flSample = 0;
		for (const Track& track : tracks)
		{
			if (track.getNumOfSamples() > i)
			{
				flSample += track.getSample(i);
			}
		}
		float absSample = std::abs(flSample);
		if (absSample > maxAmplSample)
		{
			maxAmplSample = absSample;
		}
	}
	bool normalize = maxAmplSample > 1.0f;


	for (int i = 0; i < maxNumOfSamples; i++)
	{
		float flSample = 0;
		for (const Track& track : tracks)
		{
			if (track.getNumOfSamples() > i)
			{
			flSample += track.getSample(i);
			}
		}
		if (normalize) flSample /= maxAmplSample;
		short shSample = (short)(std::round(flSample * 32767.0f));
		ofs.write((const char*)&shSample, sizeof(shSample));
	}

	if (ofs.fail())
	{
		throw std::runtime_error("Encountered an error when exporting the file!");
	}
}

void Project::saveProject(const char* filename) const
{
	std::ofstream ofs(filename, std::ios::binary);
	if (!ofs.is_open())
	{
		throw std::runtime_error("Cannot open file for writing");
	}

	ofs.write((const char*)&sampleRate, sizeof(sampleRate));
	unsigned allSoundsSize = allSounds.size();
	ofs.write((const char*)&allSoundsSize, sizeof(allSoundsSize));
	for (const Sound* sound : allSounds)
	{
		sound->save(ofs,allSounds);
	}
	unsigned allTracksSize = tracks.size();
	ofs.write((const char*)&allTracksSize, sizeof(allTracksSize));
	for (const Track& track : tracks)
	{
		track.save(ofs, allSounds);
	}
	if (ofs.fail())
	{
		throw std::runtime_error("Could not save project to file!");
	}
}


//UI below
void printInfo()
{
	std::cout << "\nPress: \n" <<
		"i - to import a sound from file. \n" <<
		"g - to generate a sound. \n" <<
		"e - to apply effect. \n" <<
		"t - create track. \n" <<
		"c - copy a track \n" <<
		"x - export \n" <<
		"s - to save the project. \n" <<
		"q - to quit. \n";
}


void Project::runUI()
{
	std::cout << "Welcome to the Digital Audio Workstation! \n";
	char input;
	while (true)
	{
		printInfo();
		std::cout << "\nEnter a command: ";
		std::cin >> input;

		try
		{
			switch (input)
			{
			case 'i':
			case 'I':
			{
				Sound* sound = createFileSoundUI();
				try
				{
					checkSampleRate(sound);
					addSoundToProjectUI(sound);
				}
				catch (...)
				{
					delete sound;
					throw;
				}
				std::cout << "\nSound imported! \n";
				break;
			}
			case 'g':
			case 'G':
			{
				Sound* sound = createGeneratedSoundUI();
				try
				{
					addSoundToProjectUI(sound);
				}
				catch (...)
				{
					delete sound;
					throw;
				}
				std::cout << "\nSound generated! \n";
				break;
			}
			case 'e':
			case 'E':
				createAndApplyEffectUI();
				std::cout << "\nEffect applied! \n";
				break;
			case 't':
			case 'T':
				tracks.push_back(Track(sampleRate));
				std::cout << "\nTrack created! \n";
				break;
			case 'c':
			case 'C':
			{
				int index = askUserForTrackIndex();
				tracks.push_back(Track(tracks[index]));
				std::cout << "\nTrack copied! \n";
				break;
			}
			case 'x':
			case 'X':
			{
				exportUI();
				std::cout << "\nExporting finished! \n";
				break;
			}
			case 's':
			case 'S':
				saveProjectUI();
				std::cout << "\nProject saved! \n";
				break;
			case 'q':
			case 'Q':
				std::cout << "\nGoodbye! \n";
				return;
			default:
				std::cout << "\nInvalid command! Try again! \n \n";
				break;
			}
			std::cin.ignore(10000, '\n');
		}
		catch (const std::exception& e)
		{
			std::cout << "\nError: " << e.what() << '\n';
		}

	}
}


int Project::askUserForTrackIndex() const
{
	if (tracks.size() == 0) throw std::runtime_error("There are no tracks to choose from!");
	int input;
	std::cout << "\nEnter a Track Index: ";
	std::cin >> input;
	while (input < 0 || input >= tracks.size() ||std::cin.fail())
	{
		std::cout << "\nInvalid index! Try again: ";
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cin >> input;
	}
	return input;
}

const SoundChunk& Project::askUserToChooseASound(int trackIndex) const
{
	if (trackIndex == -1) {
		std::cout << "\nChoose a sound from the project. To do that first select the track that the sound is on. \n";
		trackIndex = askUserForTrackIndex();
		if (tracks[trackIndex].getChunksCount() == 0) throw std::runtime_error("The track you chose has no sounds!");
	}
	std::cout << "\nNow enter the index of the sound you wish to choose: ";
	int input;
	std::cin >> input;
	while (input < 0 || input >= tracks[trackIndex].getChunksCount() || std::cin.fail())
	{
		std::cout << "\nInvalid index! Try again: ";
		std::cin.ignore(10000, '\n');
		std::cin.clear();
		std::cin >> input;
	}
	return tracks[trackIndex][input];
}

bool Project::askUserToAppendOrNot() const
{
	char input;
	std::cout << "\nDo you wish to append the sound to the end of the track? If not, you can insert it at a specific sample. \n" <<
		"NOTICE: When inserting, if the sound overlaps with another sound on the track, crossfade effect will be applied. \n \n" <<
		"Type 'y' for yes or 'n' for no : ";
	while (true)
		{
		std::cin >> input;
		switch (input)
		{
		case 'y':
		case 'Y':
			return true;
		case 'n':
		case 'N':
			return false;
		default:
			std::cout << "\nInvalid input. Try again: ";
			std::cin.ignore(10000, '\n');
			break;
		}

	}
}

int Project::askUserForSampleOnTrack() const
{
	std::cout << "\nChoose a place in the track (measured in seconds): ";
	float inputInSec;
	std::cin >> inputInSec;
	while (std::cin.fail() || inputInSec < 0 || inputInSec > INT_MAX / (float)sampleRate)
	{
		std::cout << "\nInvalid input. Try again!";
		std::cin.ignore(10000, '\n');
		std::cin.clear();
		std::cin >> inputInSec;
	}
	int chosenSample = inputInSec * sampleRate;
	
	return chosenSample;
}

void Project::addSoundToProjectUI(const Sound* sound)
{
	int trackIndex = askUserForTrackIndex();
	if (askUserToAppendOrNot())
	{
		appendSoundToTrack(sound, trackIndex);
	}
	else
	{
		int sampleOnTrack = askUserForSampleOnTrack();
		addSoundToTrack(sound, trackIndex, sampleOnTrack);
	}
}

void Project::exportUI() const
{
	std::cout << "\nEnter the file name (with filepath if needed): ";
	std::string fileName;
	std::cin >> fileName;

	while (fileName.find(".wav", fileName.size() - 4) == std::string::npos)
	{
		std::cout << "\nMake sure the name ends in .wav so it can be played. Please try again: \n";
		std::cin.ignore(10000, '\n');
		std::cin >> fileName;
	}
	std::cout << "\nDo you want to export the whole project or just a specific track? \n" <<
		"Enter p - for project or t - for track: ";
	char input;
	int index;

	while (true)
	{
		std::cin >> input;
		switch (input)
		{
		case 'p':
		case 'P':
			exportProject(fileName.c_str());
			return;
		case 't':
		case 'T':
			index = askUserForTrackIndex();
			tracks[index].writeToFile(fileName.c_str());
			return;
		default:
			std::cout << "\nInvalid input. Try again: ";
			std::cin.ignore(10000, '\n');
			break;
		}
	}
}

void Project::saveProjectUI() const
{
	std::cout << "\nEnter a name for the file to save the project (with filepath if needed): ";
	std::string fileName;
	std::cin >> fileName;

	while (fileName.find(".dat", fileName.size() - 4) == std::string::npos)
	{
		std::cout << "\nMake sure the name ends in .dat and try again: \n";
		std::cin.ignore(10000, '\n');
		std::cin >> fileName;
	}
	saveProject(fileName.c_str());
}

void Project::checkSampleRate(const Sound* sound)
{
	if (sound->getSampleRate() != sampleRate)
	{
		std::cout << "\nSorry, the sound you wish to import does not have the correct sample rate and cannot be used in this project. \n";
		throw std::runtime_error("sample rate of sound is not compatable with project.");
	}
}

Sound* Project::createFileSoundUI() const
{
	std::string input;

	while (true)
	{
		std::cout << "\nEnter the name of the file (with the correct path): ";
		std::cin >> input;

		if (input.find(".wav", input.size() - 4) != std::string::npos)
		{
			return new WAVSound(input);
		}
		if (input.find(".aif", input.size() - 4) != std::string::npos)
		{
			throw std::runtime_error("Sorry, this format is not yet supported. \n");
		}
		std::cout << "\nYou did not choose a valid file format, try again. \n";
		std::cin.ignore(10000, '\n');
	}
}


Sound* Project::createGeneratedSoundUI() const
{
	std::cout << "\nEnter what type of sound you wish to generate: \n" <<
		"t - for a Tone (Sinewave). \n" <<
		"w - for White Noise. \n" <<
		"c - for a Chirp. \n" <<
		"r - for a Repeated Sound. \n";
	char input;
	float duration;
	float amp;
	const SoundChunk* soundToRep;
	while (true)
	{
	std::cin >> input;
		switch (input)
		{
		case 't':
		case 'T':
			std::cout << "\nEnter a duration (in seconds), a wave frequency (1 - 20 000) and an amplitude (0 - 1): ";
			int freq;
			std::cin >> duration >> freq >> amp;
			return new Tone(duration, sampleRate, freq, amp);
		case 'w':
		case 'W':
			std::cout << "\nEnter a duration (in seconds) and an amplitude (0 - 1): ";
			float amp;
			std::cin >> duration >> amp;
			return new WhiteNoise(duration, sampleRate, amp);
		case 'c':
		case 'C':
			std::cout << "\nEnter a duration (in seconds), start wave frequency, end wave frequency (1 - 20 000) and an amplitude (0 - 1): ";
			int freq1, freq2;
			std::cin >> duration >> freq1 >> freq2 >> amp;
			return new Chirp(duration, sampleRate, amp, freq1, freq2);
		case 'r':
		case 'R':
			soundToRep = &askUserToChooseASound();
			std::cout << "\nEnter how many times you want to repeat the sound: ";
			int timesToRep;
			std::cin >> timesToRep;
			return new RepeatedSound(sampleRate, *soundToRep, timesToRep);
		default:
			std::cout << "\nInvalid Choice. Try again: ";
			std::cin.ignore(10000, '\n');
			break;
		}
	}
}


void Project::createAndApplyEffectUI()
{
	std::cout << "\nFirst select the track you want to modify. \n";
	int trackIndex = askUserForTrackIndex();
	std::vector<SoundChunk> baseChunks;
	Track& track = tracks[trackIndex];
	std::cout << "\nIf you want to apply it to the whole track - enter t. \n" <<
		"If you want to select a specific sound to apply to - enter s \n"
		"If you want to select a custom start and end time - enter c \n";
	int startSample;
	int endSample;
	char input;
	Sound* effect;
	while (true)
	{
		std::cin >> input;
		switch (input)
		{
		case 't':
		case 'T':
			baseChunks = track.getSoundsArr();
			effect = createEffectFromBaseSoundUI(baseChunks, tracks[trackIndex].getDuration());
			try
			{
			track.addSound(*effect, 0);
			allSounds.push_back(effect);
			}
			catch (...)
			{
				delete effect;
				throw;
			}
			return;
		case 's':
		case 'S':
		{
			const SoundChunk& sound = askUserToChooseASound(trackIndex);
			baseChunks.push_back(sound);
			effect = createEffectFromBaseSoundUI(baseChunks, sound.getNumOfSamples() / (float)sampleRate);
			try
			{
			track.addSound(*effect, const_cast<SoundChunk&>(sound));
			allSounds.push_back(effect);
			}
			catch (const std::exception&)
			{
				delete effect;
				throw;
			}
			return;
		}
		case 'c':
		case 'C':
			startSample = askUserForSampleOnTrack();
			endSample = askUserForSampleOnTrack();
			try
			{
			baseChunks = track.getChunks(startSample, endSample);
			}
			catch (...)
			{
				throw std::runtime_error("You did not enter valid start and end time.");
			}
			effect = createEffectFromBaseSoundUI(baseChunks, (endSample - startSample) / (float)sampleRate);
			try
			{
			track.addSound(*effect, startSample);
			allSounds.push_back(effect);
			}
			catch (const std::exception&)
			{
				delete effect;
				throw;
			}
			return;
		default:
			std::cout << "\nInvalid Choice. Try again : ";
			std::cin.ignore(10000, '\n');
			break;
		}
	}
}
Sound* Project::createEffectFromBaseSoundUI(std::vector<SoundChunk> baseSounds, float duration) const
{
	std::cout << "\nEnter what type of effect you wish to create: \n" <<
		"a - to amplify. \n" <<
		"h - to apply a high-pass filter. \n";

	char input;
	while (true)
	{
		std::cin >> input;
		switch (input)
		{
		case 'a':
		case 'A':
			std::cout << "\nEnter decibels (-30 - 30): ";
			float decibels;
			std::cin >> decibels;
			return new Amplify(duration, sampleRate, baseSounds, decibels);
		case 'h':
		case 'H':
			std::cout << "\nEnter a cutoff frequency (1 - 20 000): ";
			int cutoffFreq;
			std::cin >> cutoffFreq;
			throw std::runtime_error("Sorry, high-pass filter is not implemented yet."); //return new HighPassFilter(duration, sampleRate, baseSounds, cutoffFreq);
		default:
			std::cout << "\nInvalid Choice. Try again : ";
			std::cin.ignore(10000, '\n');
			break;
		}
	}
}