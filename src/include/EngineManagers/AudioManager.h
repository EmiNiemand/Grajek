//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AUDIOMANAGER_H
#define GLOOMENGINE_AUDIOMANAGER_H

#include <al.h>
#include <alc.h>
#include <alext.h>
#include <memory>
#include <unordered_map>
#include <thread>

class GloomEngine;
class AudioSource;
class AudioListener;

class AudioManager {
    ALCdevice* audioDevice;
    ALCcontext* audioContext;
    std::jthread deviceChanger;

    inline static AudioManager* audioManager;
    explicit AudioManager();
    static void CheckAudioDevice(const std::stop_token& token, ALCdevice** device);

public:
    std::shared_ptr<AudioListener> audioListener;
    std::unordered_map<uint32_t, std::shared_ptr<AudioSource>> audioSources;

//    Currently unused, maybe in future for audio streaming
//    std::unordered_map<ALuint, std::shared_ptr<AudioSource>> audioBuffers;

    AudioManager(AudioManager &other) = delete;
    void operator=(const AudioManager&) = delete;
    virtual ~AudioManager();

    static AudioManager* GetInstance();
    void Free();

    void InitializeAudio();
    void RemoveAudioSource(uint32_t componentId);

};

#endif //GLOOMENGINE_AUDIOMANAGER_H
