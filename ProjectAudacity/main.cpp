#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Sound/Silence/Silence.h"
#include "Sound/GeneratedSounds/Tone.h"
#include "Track/Track.h"
#include "Sound/EffectSounds/Amplify.h"
#include"Sound/FileSounds/WAVHeader.h"
#include "Sound/GeneratedSounds/WhiteNoise.h"

int main()
{
	//setup random
	std::srand((unsigned)std::time(0));

	WhiteNoise wn1(2, 44100, 0.3);
	Tone t1(8, 44100, 6000, 0.3);
	Tone t2(5, 44100, 1000, 0.5);
	Tone t3(1, 44100, 10000, 0.5);

	Track tr(44100);
	tr.addSound(t1);
	tr.addSound(t2, 44100*2);
	tr.addSound(wn1, 44100*2);
	tr.addSound(wn1, 44100*4);

	SoundChunk& chunkToReplace = tr[1];
	std::vector<SoundChunk> chunks;
	chunks.push_back(chunkToReplace);

	Amplify a1(2,44100, chunks, 5);
	tr.addSound(a1,chunkToReplace);

	tr.writeToFile("test1.wav");



}