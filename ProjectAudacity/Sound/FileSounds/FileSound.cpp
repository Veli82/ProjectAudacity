#include "FileSound.h"
#include <stdexcept>

FileSound::FileSound(const std::string& filePath)   //there cant be objects from this abstract class so the error shouldnt be worrying
    :Sound(0, 0), filePath(filePath)
{
    reader = new std::ifstream(filePath, std::ios::binary);
    if (!reader->is_open())
    {
        delete reader;
        throw std::runtime_error("Could not open file!");
    }

}

FileSound::~FileSound()
{
    reader->close();
    delete reader;
}

const std::string& FileSound::getFilePath() const
{
    return filePath;
}
