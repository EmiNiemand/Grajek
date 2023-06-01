//
// Created by Adrian on 07.04.2023.
//

#ifndef GLOOMENGINE_AUDIOLOADER_H
#define GLOOMENGINE_AUDIOLOADER_H

#include "ProjectSettings.h"
#include <vector>
#include <fstream>
#include <al.h>
#include <alext.h>

constexpr int BUFFER_SIZE = 65536;

class AudioLoader {
    ALenum format = 0;
    ALuint source = 0;
    const ALuint *buffers = nullptr;
    // Header values of WAV file
    std::ifstream file {};
    long long filePointer = 0;
    uint32_t fileSize = 0;
    uint32_t formatLength = 0;
    uint16_t formatType = 0;
    uint16_t numChannels = 0;
    ALsizei sampleRate = 0;
    uint32_t byteRate = 0;
    uint16_t blockAlign = 0;
    uint16_t bitsPerSample = 0;
    ALsizei subChunkSize = 0;
    // Remaining amount of data to load
    ALsizei samplesSizeToLoad = 0;

public:
    explicit AudioLoader();
    virtual ~AudioLoader();

    void InitializeAudioLoader(const ALuint &sourceId, const ALuint *buffersId);
    void OpenFile(const std::string &path);
    void LoadFileHeader(const AudioType& type);
    void FillBuffersQueue();
    const bool FillProcessedBuffers(const ALuint &processedBuffers);
    void CloseFile();

};



#endif //GLOOMENGINE_AUDIOLOADER_H
