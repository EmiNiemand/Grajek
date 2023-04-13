//
// Created by Adrian on 07.04.2023.
//
#include "Components/Scripts/AudioLoader.h"
#include "spdlog/spdlog.h"
#include <AudioFile.h>
#include <al.h>

AudioLoader::AudioLoader() = default;

AudioLoader::~AudioLoader() = default;

void AudioLoader::LoadAudioFile(const char* path, AudioFileType type) {
    if (type == AudioFileType::mp3) {
        LoadMp3(path);
    } else if (type == AudioFileType::wav) {
        LoadWav(path);
    }
}

void AudioLoader::LoadWav(const char* path) {
    AudioFile<int> file;

    if (!file.load(path))
        spdlog::info("Error loading sound in ", path);

    this->sampleRate = file.getSampleRate();
    this->bitsPerSample = file.getBitDepth();

    for (auto &&data: file.samples) {
        for (auto &&sample: data) {
            this->soundData.push_back(sample);
        }
    }

    int channels = file.getNumChannels();

    if (channels == 1 && bitsPerSample == 8) {
        format = AL_FORMAT_MONO8;
    } else if (channels == 1 && bitsPerSample == 16) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2 && bitsPerSample == 8) {
        format = AL_FORMAT_STEREO8;
    } else if (channels == 2 && bitsPerSample == 16) {
        format = AL_FORMAT_STEREO16;
    }
}

void AudioLoader::LoadMp3(const char* path) {

}