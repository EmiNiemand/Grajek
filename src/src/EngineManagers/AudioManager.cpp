//
// Created by Adrian on 05.04.2023.
//

#include "EngineManagers/AudioManager.h"
#include "Components/Audio/AudioSource.h"
#include "spdlog/spdlog.h"
#include <al.h>
#include <alc.h>
#include <alext.h>
#include <string>

AudioManager::AudioManager() = default;

AudioManager::~AudioManager() = default;

AudioManager* AudioManager::GetInstance() {
    return (audioManager == nullptr) ? audioManager = new AudioManager() : audioManager;
}

void AudioManager::InitializeAudio() {
    audioDevice = alcOpenDevice(nullptr);

    if (audioDevice) {
        audioContext = alcCreateContext(audioDevice, nullptr);
        alcMakeContextCurrent(audioContext);
    }

    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

    spdlog::info("Successfully initialized OpenAL-Soft on " +
                (std::string)alcGetString(audioDevice, ALC_ALL_DEVICES_SPECIFIER));
}

void AudioManager::RemoveAudioSource(int componentId) {
    if (audioSources.contains(componentId))
        audioSources.erase(componentId);
}

void AudioManager::Free() {
    for (auto&& source: audioSources) {
        source.second->Free();
    }

    audioSources.clear();

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);

    audioDevice = nullptr;
    audioContext = nullptr;
}
