//
// Created by Adrian on 07.04.2023.
//

#include "Components/Audio/AudioLoader.h"
#include "Components/Audio/AudioSource.h"
#include <spdlog/spdlog.h>

AudioLoader::AudioLoader() = default;

AudioLoader::~AudioLoader() = default;

/**
 * @annotation
 * Sets source and buffers ids.
 * @param sourceId - source id
 * @param buffersId - pointer to array of buffers ids
 */
void AudioLoader::InitializeAudioLoader(const ALuint &sourceId, const ALuint *buffersId) {
    source = sourceId;
    buffers = buffersId;
}

/**
 * @annotation
 * Opens audio file and checks for any errors.
 * @param path - path to .wav file
 */
void AudioLoader::OpenFile(const std::string &path) {
    file.open(path, std::ios::binary);

    if (file.bad()) {
        spdlog::error(path + " is not a correct .WAV file format!");
        return;
    }

    if (!file.is_open()) {
        spdlog::error(path + " is already opened, did you forget to close it?");
        return;
    }
}

/**
 * @annotation
 * Loads file header, e.g. audio file parameters.
 * @param type - Positional or Direct audio
 */
void AudioLoader::LoadFileHeader(const AudioType& type) {
    if (!file.is_open()) {
        spdlog::error("File is not opened, did you forget to call another function?");
        return;
    }

    char data[4];

    //
    // "RIFF" header
    //
    if (!file.read(data, 4)) {
        spdlog::error("Invalid RIFF header!");
        return;
    }

    if (std::strncmp(data, "RIFF", 4) != 0) {
        spdlog::error("Invalid WAVE file (header doesn't contain RIFF)!");
        return;
    }

    if (!file.read(reinterpret_cast<char *>(&fileSize), 4)) {
        spdlog::error("Invalid file size!");
        return;
    }

    if (!file.read(data, 4)) {
        spdlog::error("Invalid WAVE header!");
        return;
    }

    if (std::strncmp(data, "WAVE", 4) != 0) {
        spdlog::error("Invalid WAVE file (header doesn't contain WAVE)!");
        return;
    }

    //
    // "fmt" header
    //
    if (!file.read(data, 4)) {
        spdlog::error("Invalid read fmt header!");
        return;
    }

    if (std::strncmp(data, "fmt ", 4) != 0) {
        spdlog::error("Invalid WAVE file (header doesn't contain fmt )!");
        return;
    }

    if (!file.read(reinterpret_cast<char *>(&formatLength), 4)) {
        spdlog::error("Invalid fmt data chunk size!");
        return;
    }

    if (!file.read(reinterpret_cast<char *>(&formatType), 2)) {
        spdlog::error("Invalid format type!");
        return;
    }

    if (!file.read(reinterpret_cast<char *>(&numChannels), 2)) {
        spdlog::error("Invalid number of channels!");
        return;
    }

    if (!file.read(reinterpret_cast<char *>(&sampleRate), 4)) {
        spdlog::error("Invalid sample rate!");
        return;
    }

    // SampleRate * NumChannels * BitsPerSample / 8
    if (!file.read(reinterpret_cast<char *>(&byteRate), 4)) {
        spdlog::error("Invalid byte rate!");
        return;
    }

    if (!file.read(reinterpret_cast<char *>(&blockAlign), 2)) {
        spdlog::error("Invalid block align!");
        return;
    }

    if (!file.read(reinterpret_cast<char *>(&bitsPerSample), 2)) {
        spdlog::error("Invalid bits per sample!");
        return;
    }

    //
    // "data" header
    //
    if (!file.read(data, 4)) {
        spdlog::error("Invalid data header!");
        return;
    }

    if (std::strncmp(data, "data", 4) != 0) {
        spdlog::error("Invalid WAVE file (header doesn't contain data)!");
        return;
    }

    if (!file.read(reinterpret_cast<char *>(&subChunkSize), 4)) {
        spdlog::error("Invalid SubChunk size!");
        return;
    }

    if (file.eof()) {
        spdlog::error("Reached end of file!");
        return;
    }

    dataStartSectionPointer = file.tellg();

    if (type == AudioType::Positional) {
        if (bitsPerSample == 8)
            format = AL_FORMAT_MONO8;
        else if (bitsPerSample == 16)
            format = AL_FORMAT_MONO16;
        else
            format = AL_FORMAT_MONO_FLOAT32;
    } else {
        if (bitsPerSample == 8)
            format = AL_FORMAT_STEREO8;
        else if (bitsPerSample == 16)
            format = AL_FORMAT_STEREO16;
        else
            format = AL_FORMAT_STEREO_FLOAT32;
    }

    samplesSizeToLoad = subChunkSize;
}

/**
 * @annotation
 * Initializes buffers queue and loads first chunks of data.
 */
void AudioLoader::FillBuffersQueue() {
    ALfloat* data = new ALfloat[BUFFER_SIZE];

    for (int i = 0; i < NUM_BUFFERS; ++i) {
        if (samplesSizeToLoad < BUFFER_SIZE) {
            file.read(reinterpret_cast<char *>(data), samplesSizeToLoad);
            file.seekg(dataStartSectionPointer);
            alBufferData(buffers[i], format, data, samplesSizeToLoad, sampleRate);
            alSourceQueueBuffers(source, 1, &buffers[i]);
            samplesSizeToLoad = subChunkSize;
            break;
        } else {
            file.read(reinterpret_cast<char *>(data), BUFFER_SIZE);
            alBufferData(buffers[i], format, data, BUFFER_SIZE, sampleRate);
            alSourceQueueBuffers(source, 1, &buffers[i]);
            samplesSizeToLoad -= BUFFER_SIZE;
        }
    }

    delete []data;
}

/**
 * @annotation
 * Fills processed buffers with next chunks of data and adds them to queue.
 * @param processedBuffers - amount of already processed buffers
 * @returns bool - true if there is more data to load, otherwise false
 */
const bool AudioLoader::FillProcessedBuffers(const ALuint &processedBuffers) {
    ALfloat* data = new ALfloat[BUFFER_SIZE];
    ALuint bufferId;

    bool isPlaying = true;

    for (int i = 0; i < processedBuffers; ++i) {
        alSourceUnqueueBuffers(source, 1, &bufferId);

        if (samplesSizeToLoad < BUFFER_SIZE) {
            file.read(reinterpret_cast<char *>(data), samplesSizeToLoad);
            file.seekg(dataStartSectionPointer);
            alBufferData(bufferId, format, data, samplesSizeToLoad, sampleRate);
            alSourceQueueBuffers(source, 1, &bufferId);
            samplesSizeToLoad = subChunkSize;
            isPlaying = false;
            break;
        } else {
            file.read(reinterpret_cast<char *>(data), BUFFER_SIZE);
            alBufferData(bufferId, format, data, BUFFER_SIZE, sampleRate);
            alSourceQueueBuffers(source, 1, &bufferId);
            samplesSizeToLoad -= BUFFER_SIZE;
        }
    }

    delete []data;
    return isPlaying;
}

/**
 * @annotation
 * Closes audio file.
 */
void AudioLoader::CloseFile() {
    if (file.is_open())
        file.close();
}

