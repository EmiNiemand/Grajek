//
// Created by Adrian on 05.04.2023.
//

#include "EngineManagers/AudioManager.h"
#include "Components/Audio/AudioSource.h"
#include "spdlog/spdlog.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

// Macro copied from cmake-build-release/_deps/openal-soft-src/alc/inprogext.h
#define AL_STOP_SOURCES_ON_DISCONNECT_SOFT 0x19AB

// NOTE:: alcIsExtensionPresent(audioDevice, "ALC_SOFT_reopen_device")
// Returns boolean, checks whether the given extension is present.

AudioManager::AudioManager() = default;

AudioManager::~AudioManager() {
    delete audioManager;
}

AudioManager* AudioManager::GetInstance() {
    return (audioManager == nullptr) ? audioManager = new AudioManager() : audioManager;
}

/**
 * @annotation
 * Initializes OpenAL device and context. Also creates new thread
 * which periodically checks if the device is connected/disconnected.
 */
void AudioManager::InitializeAudio() {
#ifdef DEBUG
    ZoneScopedNC("AudioManager", 0xDC143C);
#endif

    audioDevice = alcOpenDevice(nullptr);

    if (audioDevice) {
        audioContext = alcCreateContext(audioDevice, nullptr);
        alcMakeContextCurrent(audioContext);

        SetAudioSettings();

        spdlog::info("Successfully initialized OpenAL-Soft on " +
                     (std::string)alcGetString(audioDevice, ALC_ALL_DEVICES_SPECIFIER));

        isInitialized = true;
    }

    deviceChanger = std::jthread(CheckAudioDevice, &audioDevice, std::ref(isInitialized));
}

void AudioManager::SetAudioSettings() {
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    alDisable(AL_STOP_SOURCES_ON_DISCONNECT_SOFT);
}

/**
 * @annotation
 * Removed audio source from manager.
 * @param componentId - component id
 */
void AudioManager::RemoveAudioSource(int componentId) {
    if (audioSources.contains(componentId)) {
        audioSources.at(componentId)->Free();
        audioSources.erase(componentId);
    }
}

void AudioManager::Free() {
    deviceChanger.request_stop();
    deviceChanger.join();

    for (auto&& source: audioSources)
        source.second->Free();

    audioSources.clear();

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);

    audioDevice = nullptr;
    audioContext = nullptr;
}

/**
 * @annotation
 * Checks every 500ms if the audio device is properly connected.
 * @param token - stop token
 * @param device - pointer to device pointer
 */
void AudioManager::CheckAudioDevice(const std::stop_token& token, ALCdevice** device, bool& isInitialized) {
    using namespace std::chrono;

    ALCint status;
    auto alcReopenDeviceSOFT = (LPALCREOPENDEVICESOFT)alcGetProcAddress(*device, "alcReopenDeviceSOFT");
    auto alcResetDeviceSOFT = (LPALCRESETDEVICESOFT)alcGetProcAddress(*device, "alcResetDeviceSOFT");

    while(!token.stop_requested()) {
        alcGetIntegerv(*device, ALC_CONNECTED, 1, &status);

        if (!status) {
            spdlog::error("Audio device disconnected! Trying to reconnect...");

            if (!alcResetDeviceSOFT(*device, nullptr)) {
                spdlog::error("Audio device reconnecting failed! Trying to open new device...");

                if (!alcReopenDeviceSOFT(*device, nullptr, nullptr)) {
                    spdlog::error("Opening new audio device failed! Trying again in 500 ms...");
                } else {
                    spdlog::info("Successfully opened new device on " +
                                 (std::string) alcGetString(*device, ALC_ALL_DEVICES_SPECIFIER));

                    if (!isInitialized) {
                        isInitialized = true;
                        SetAudioSettings();
                    }
                }
            } else {
                spdlog::info("Successfully reconnected OpenAL-Soft on " +
                             (std::string)alcGetString(*device, ALC_ALL_DEVICES_SPECIFIER));

                if (!isInitialized) {
                    isInitialized = true;
                    SetAudioSettings();
                }
            }
        }

        std::this_thread::sleep_for(500ms);
    }
}
