//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AUDIOMANAGER_H
#define GLOOMENGINE_AUDIOMANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <thread>
#include <al.h>
#include <alc.h>
#include <alext.h>

class GloomEngine;
class AudioSource;
class AudioListener;

class AudioManager {
    bool isInitialized = false;
    ALCdevice* audioDevice = nullptr;
    ALCcontext* audioContext = nullptr;
    std::jthread deviceChanger;

    inline static AudioManager* audioManager;
    explicit AudioManager();
    static void CheckAudioDevice(const std::stop_token& token, ALCdevice* device, bool& isInitialized);
    static void SetAudioSettings();

public:
    std::shared_ptr<AudioListener> audioListener;
    std::unordered_map<int, std::shared_ptr<AudioSource>> audioSources;

    AudioManager(AudioManager &other) = delete;
    void operator=(const AudioManager&) = delete;
    virtual ~AudioManager();

    static AudioManager* GetInstance();
    void Free();

    void InitializeAudio();
    void RemoveAudioSource(int componentId);

};

#endif //GLOOMENGINE_AUDIOMANAGER_H
