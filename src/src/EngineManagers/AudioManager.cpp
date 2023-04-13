//
// Created by Adrian on 05.04.2023.
//

#include "EngineManagers/AudioManager.h"
#include "spdlog/spdlog.h"
#include <al.h>
#include <alc.h>

AudioManager::AudioManager() = default;

AudioManager::~AudioManager() = default;

AudioManager* AudioManager::GetInstance() {
    return (audioManager == nullptr) ? new AudioManager() : audioManager;
}

void AudioManager::InitializeAudio() {
    audioDevice = std::make_shared<ALCdevice*>(alcOpenDevice(nullptr));

    if (audioDevice) {
        audioContext = std::make_shared<ALCcontext*>(alcCreateContext(*audioDevice, nullptr));
        alcMakeContextCurrent(*audioContext);
    }
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
