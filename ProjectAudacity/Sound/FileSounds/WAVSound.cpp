#include "WAVSound.h"
#include "WAVHeader.h"

#include <cstring>
#include <iostream> //mahni posle
#include <cmath>

//try to split the functionalities into multiple functions!


//for debug
void print(std::ifstream& reader, int size)
{
    char byte;
    reader.seekg(0, std::ios::beg);
    for (int i = 0; i < size; i++)
    {
        reader.read((char*)&byte, 1);
        std::cout << (int)byte << ' ';
    }
}


WAVSound::WAVSound(const std::string& filePath)
    :FileSound(filePath)
{
    //read header
    WAVHeader header;
    reader.read((char*)&header, sizeof(header));

    //is it nessesary?
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

    //check for mono or stereo
    if (header.numChannels == 1)
    {
        isStereo = false;
    }
    else if (header.numChannels == 2)
    {
        isStereo = true;
    }
    else
    {
        throw std::runtime_error("Only Mono and Stereo audio is supported.");
    }

    //assignment
    blockAlign = header.blockAlign;
    bitsPerSample = header.bitsPerSample;
    sampleRate = header.sampleRate;

    //searching for data chunk
    char buffer[4];
    int size = 0;
    reader.read(buffer, 4);
    reader.read((char*)&size, 4);

    //skipping if not data chunk
    while (strncmp(buffer, "data", 4))
    {
        reader.seekg(size, std::ios::cur);
        reader.read(buffer, 4);
        reader.read((char*)&size, 4);
    }

    //more assignment
    numOfSamples = size / (header.numChannels * bitsPerSample / 8);
    duration = (float)numOfSamples / sampleRate;

    firstSamplePos = reader.tellg();  
}

float WAVSound::getSample(int index) const
{
    validateIndex(index);

    short sample1 = 0;
    short sample2 = 0;
    short result;

    reader.seekg(firstSamplePos + index * blockAlign);
    reader.read((char*)&sample1, bitsPerSample / 8);

    if (isStereo)
    {
        reader.read((char*)&sample2, bitsPerSample/8);
        result = (sample1 + sample2) / 2;
    }
    else
    {
        result = sample1;
    }


    if (reader.fail())
    {
        throw std::runtime_error(std::string("Error! Could not read from file: ") + filePath);    //test this
    }

    //convert samples from int to float
    if (bitsPerSample == 8)
    {
         return (result - 128) / 128.0f;
    }
    else
    {
        return result / std::pow(2, bitsPerSample-1);
    }    
}

