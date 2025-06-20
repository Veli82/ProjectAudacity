#include "FileSound.h"
#include <stdexcept>

FileSound::FileSound(const std::string& filePath)   //there cant be objects from this abstract class so the warning shouldn't be worrying
    :Sound(0, 1), filePath(filePath)
{
    reader.open(filePath, std::ios::binary);
    if (!reader.is_open())
    {
        throw std::runtime_error("Could not open that file!");
    }

}

FileSound::FileSound(const FileSound& other)
    :Sound(other.duration, other.sampleRate), filePath(other.filePath), isStereo(other.isStereo),
    blockAlign(other.blockAlign), bitsPerSample(other.bitsPerSample), firstSamplePos(other.firstSamplePos)
{
    reader.open(filePath, std::ios::binary);
    if (!reader.is_open())
    {
        throw std::runtime_error("Could not open that file!");
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

void FileSound::save(std::ofstream& ofs, const std::vector<const Sound*>& sounds) const
{
    unsigned length = filePath.size();
    ofs.write((const char*)(&length), sizeof(length));
    ofs.write(filePath.c_str(), length);
}

//read
//size_t len;
//ifs.read((char*)(&len), sizeof(len));
//std::string filePath(len, '\0');
//ifs.read(&filePath[0], len);