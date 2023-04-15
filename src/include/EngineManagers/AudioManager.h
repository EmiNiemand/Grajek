//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AUDIOMANAGER_H
#define GLOOMENGINE_AUDIOMANAGER_H

#include <al.h>
#include <alc.h>
#include <memory>
#include <unordered_map>

class GloomEngine;
class AudioSource;
class AudioListener;

class AudioManager {
    inline static AudioManager* audioManager;
    explicit AudioManager();

public:
    std::shared_ptr<AudioListener> audioListener;
    std::unordered_map<int, std::shared_ptr<AudioSource>> audioSources;
//    Currently unused, maybe in future for audio streaming
//    std::unordered_map<ALuint, std::shared_ptr<AudioSource>> audioBuffers;
    std::unique_ptr<ALCdevice*> audioDevice;
    std::unique_ptr<ALCcontext*> audioContext;

    AudioManager(AudioManager &other) = delete;
    void operator=(const AudioManager&) = delete;
    virtual ~AudioManager();

    static AudioManager* GetInstance();
    void Free();

    void InitializeAudio();
    void RemoveAudioSource(int componentId);

};

#endif //GLOOMENGINE_AUDIOMANAGER_H
