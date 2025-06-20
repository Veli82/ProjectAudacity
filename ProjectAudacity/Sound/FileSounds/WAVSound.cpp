#include "WAVSound.h"
#include "WAVHeader.h"
#include "../readAndWriteUtils.h"

#include <cstring>
#include <cmath>



WAVSound::WAVSound(const std::string& filePath)
    :FileSound(filePath)
{
    //read header
    WAVHeader header;
    readWavHeader(reader, header);

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

void WAVSound::save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
{
    SoundType type = SoundType::WAVSound;
    ofs.write((const char*)&type, sizeof(type));
    FileSound::save(ofs, sounds);
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

