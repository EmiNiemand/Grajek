//
// Created by Adrian on 07.04.2023.
//

#include "LowLevelClasses/AudioLoader.h"
#include "ProjectSettings.h"
#include <spdlog/spdlog.h>
#include <AudioFile.h>
#include <al.h>

AudioLoader::AudioLoader() = default;

AudioLoader::~AudioLoader() = default;

void AudioLoader::LoadAudioFile(const char* path, AudioType type, ALuint bufferId) {
    AudioFile<ALshort> file;
    std::vector<ALshort> soundData;
    ALenum format;
    int sampleRate;
    int bitsPerSample;

    if (!file.load(path)) {
        spdlog::error("ERROR! Failed loading sound ", path);
        alBufferData(bufferId, NULL, nullptr, NULL, NULL);
        return;
    }

    sampleRate = file.getSampleRate();
    bitsPerSample = file.getBitDepth();

    int channels = file.getNumChannels();

    for (int i = 0; i < file.getNumSamplesPerChannel(); i++) {
        for (int j = 0; j < channels; j++) {
            soundData.push_back(file.samples[j][i]);
        }
    }

    if (type == AudioType::Positional && bitsPerSample == 8) {
        format = AL_FORMAT_MONO8;
    } else if (type == AudioType::Positional && bitsPerSample == 16) {
        format = AL_FORMAT_MONO16;
    } else if (type == AudioType::Direct && bitsPerSample == 8) {
        format = AL_FORMAT_STEREO8;
    } else if (type == AudioType::Direct && bitsPerSample == 16) {
        format = AL_FORMAT_STEREO16;
    }

    alBufferData(bufferId, format, soundData.data(), soundData.size() * 2, sampleRate);
}
