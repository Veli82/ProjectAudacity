#include "WAVHeader.h"

void writeWavHeader(std::ofstream& ofs, int numOfSamples, int sampleRate, short numOfChannels, short bitsPerSample)
{
	int subchunk2Size = numOfSamples * numOfChannels * (bitsPerSample / 8);
	WAVHeader header;
	strncpy(header.riffHeader, "RIFF", 4);
	header.fileSize = 36 + subchunk2Size;
	strncpy(header.waveHeader, "WAVE", 4);
	strncpy(header.fmtHeader, "fmt ", 4);
	header.fmtChunkSize = 16;
	header.audioFormat = 1;
	header.numChannels = numOfChannels;
	header.sampleRate = sampleRate;
	header.byteRate = sampleRate * numOfChannels * (bitsPerSample / 8);
	header.blockAlign = numOfChannels * (bitsPerSample / 8);
	header.bitsPerSample = bitsPerSample;

	assert(sizeof(header) == 36);

	char subchunk2Header[4];
	strncpy(subchunk2Header, "data", 4);

	ofs.write((const char*)&header, sizeof(header));
	ofs.write((const char*)&subchunk2Header, sizeof(subchunk2Header));
	ofs.write((const char*)&subchunk2Size, sizeof(subchunk2Size));
}

void readWavHeader(std::ifstream& reader, WAVHeader& header)
{
	reader.read((char*)&header, sizeof(header));

	if (reader.fail())
	{
		throw std::runtime_error("Error! Could not read from this file.");
	}

	//file validation
	if (strncmp(header.riffHeader, "RIFF", 4) ||
		strncmp(header.waveHeader, "WAVE", 4) ||
		strncmp(header.fmtHeader, "fmt ", 4) ||
		header.sampleRate <= 0 ||
		header.bitsPerSample <= 0 ||
		header.bitsPerSample % 8 != 0 ||
		header.blockAlign != header.numChannels * header.bitsPerSample / 8)
	{
		throw std::runtime_error("File is not in the right format.");
	}

	//check for PCM
	if (header.audioFormat != 1)
	{
		throw std::runtime_error("Audio compression is not supported.");
	}
}
