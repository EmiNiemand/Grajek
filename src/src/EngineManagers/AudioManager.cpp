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

// Macro copied from cmake-build-release/_deps/openal-soft-src/alc/inprogext.h
#define AL_STOP_SOURCES_ON_DISCONNECT_SOFT 0x19AB

// NOTE:: alcIsExtensionPresent(audioDevice, "ALC_SOFT_reopen_device")
// Returns boolean, checks whether the given extension is present.

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
    alDisable(AL_STOP_SOURCES_ON_DISCONNECT_SOFT);

    spdlog::info("Successfully initialized OpenAL-Soft on " +
                (std::string)alcGetString(audioDevice, ALC_ALL_DEVICES_SPECIFIER));

    deviceChanger = std::jthread(CheckAudioDevice, &audioDevice);
}

void AudioManager::RemoveAudioSource(int componentId) {
    if (audioSources.contains(componentId)) {
        audioSources.at(componentId)->Free();
        audioSources.erase(componentId);
    }
}

void AudioManager::Free() {
    deviceChanger.request_stop();
    deviceChanger.join();

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

void AudioManager::CheckAudioDevice(const std::stop_token& token, ALCdevice** device) {
    using namespace std::chrono;

    ALCint status;
    static auto alcReopenDeviceSOFT = (LPALCREOPENDEVICESOFT)alcGetProcAddress(*device, "alcReopenDeviceSOFT");
    static auto alcResetDeviceSOFT = (LPALCRESETDEVICESOFT)alcGetProcAddress(*device, "alcResetDeviceSOFT");

    while(true) {
        alcGetIntegerv(*device, ALC_CONNECTED, 1, &status);

        if (!status)
        {
            spdlog::error("Audio device disconnected! Trying to reconnect...");

            if (!alcResetDeviceSOFT(*device, nullptr))
            {
                spdlog::error("Audio device reconnecting failed! Trying to open new device...");

                if (!alcReopenDeviceSOFT(*device, nullptr, nullptr)) {
                    spdlog::error("Opening new audio device failed! Trying again in 500 ms...");
                } else {
                    spdlog::info("Successfully opened new device on " +
                                 (std::string)alcGetString(*device, ALC_ALL_DEVICES_SPECIFIER));
                }
            } else {
                spdlog::info("Successfully reconnected OpenAL-Soft on " +
                             (std::string)alcGetString(*device, ALC_ALL_DEVICES_SPECIFIER));
            }
        }

        if (token.stop_requested()) {
            return;
        }

        std::this_thread::sleep_for(500ms);
    }
}

