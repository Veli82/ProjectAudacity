#pragma once

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