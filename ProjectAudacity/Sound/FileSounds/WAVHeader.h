#pragma once
#include <fstream>
#include <cstring>
#include <cassert>
#pragma warning (disable : 4996)


struct WAVHeader {
	char riffHeader[4];
	int fileSize;
	char waveHeader[4];

	char fmtHeader[4];
	int fmtChunkSize;
	short audioFormat;
	short numChannels;
	int sampleRate;
	int byteRate;
	short blockAlign;
	short bitsPerSample;
};

void writeWavHeader(std::ofstream& ofs, int numOfSamples, int sampleRate, short numOfChannels, short bitsPerSample);

void readWavHeader(std::ifstream& reader, WAVHeader& header);