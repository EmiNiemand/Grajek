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
    return (audioManager == nullptr) ? new AudioManager() : audioManager;
}

void AudioManager::InitializeAudio() {
    audioDevice = std::make_unique<ALCdevice*>(alcOpenDevice(nullptr));

    if (audioDevice) {
        audioContext = std::make_unique<ALCcontext*>(alcCreateContext(*audioDevice, nullptr));
        alcMakeContextCurrent(*audioContext);
    }

    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

    spdlog::info("Successfully initialized OpenAL-Soft on " +
                (std::string)alcGetString(*audioDevice, ALC_ALL_DEVICES_SPECIFIER));
}

void AudioManager::RemoveAudioSource(int componentId) {
    if (audioSources.contains(componentId))
        audioSources.erase(componentId);
}

void AudioManager::Free() {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(*audioContext);
    alcCloseDevice(*audioDevice);
    audioSources.clear();
    audioListener = nullptr;
    audioDevice = nullptr;
    audioContext = nullptr;
}
