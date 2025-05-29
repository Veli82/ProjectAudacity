#include "AIFFSound.h"

struct AIFFHeader
{
    char formId[4];
    char formType[4];

    char commonChunkId[4];
    int commonChunkSize;
    short numOfChannels;
    unsigned numOfSamples;
    short bitsPerSample;
    unsigned sampleRate;        //float?
    //tova e 80bits float koito ne e standartiziran i shte e pain in the ass
    //pitai da polzvash biblioteka za tova shtoto si e ebalo maikata otvsqkyde
};

AIFFSound::AIFFSound(const std::string& filePath)
    :FileSound(filePath)
{
    AIFFHeader header;
    reader->read((char*)&header, sizeof(header));

    //is it nessesary?
    if (reader->fail())
    {
        throw std::runtime_error("Error! Could not read from this file.");
    }

    if (strncmp(header.formId, "FORM", 4) ||
        strncmp(header.formType, "AIFF", 4) ||
        strncmp(header.commonChunkId, "COMM", 4) )
    {
        throw std::runtime_error("File is not in the right format.");
    }
}

float AIFFSound::getSample(int index) const
{
    validateIndex(index);
    return 0.0f;
}
