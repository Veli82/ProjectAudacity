#include "FileSound.h"
#include <stdexcept>

FileSound::FileSound(const std::string& filePath)   //there cant be objects from this abstract class so the error shouldnt be worrying
    :Sound(0, 0), filePath(filePath)
{
    reader.open(filePath, std::ios::binary);
    if (!reader.is_open())
    {
        throw std::runtime_error("Could not open file!");
    }

}

FileSound::FileSound(const FileSound& other)
    :Sound(other.duration, other.sampleRate), filePath(other.filePath), isStereo(other.isStereo),
    blockAlign(other.blockAlign), bitsPerSample(other.bitsPerSample), firstSamplePos(other.firstSamplePos)
{
    reader.open(filePath, std::ios::binary);
    if (!reader.is_open())
    {
        throw std::runtime_error("Could not open file!");
    }
}

const std::string& FileSound::getFilePath() const
{
    return filePath;
}

bool FileSound::isItStereo() const
{
    return isStereo;
}

unsigned FileSound::getBlockAlign() const
{
    return blockAlign;
}

unsigned FileSound::getBitsPerSample() const
{
    return bitsPerSample;
}
