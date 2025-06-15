#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Sound/Silence/Silence.h"
#include "Sound/GeneratedSounds/Tone.h"
#include "Track/Track.h"
#include "Sound/EffectSounds/Amplify.h"

int main()
{
	//setup random
	std::srand((unsigned)std::time(0));
	Tone t1(1, 100, 10000, 0.5);
	Tone t2(1, 100, 12000, 0.5);
	Tone t3(1, 50, 14000, 0.5);
	Tone t4(1, 100, 16000, 0.5);

	Track tr(100);
	tr.addSound(t1);
	tr.addSound(t2, 50);
	tr.addSound(t3, 75);

	//SoundChunk& chunkToReplace = tr[1];
	//std::vector<SoundChunk> chunks;
	//chunks.push_back(chunkToReplace);
	//Amplify a1(1, 50, chunks, 0.5);
	//tr.addSound(a1,chunkToReplace);

}