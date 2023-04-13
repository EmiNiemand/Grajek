//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AUDIOMANAGER_H
#define GLOOMENGINE_AUDIOMANAGER_H

#include <al.h>
#include <alc.h>
#include <vector>
#include <map>
#include <memory>

class GloomEngine;
class AudioSource;
class AudioListener;

class AudioManager {
    inline static AudioManager* audioManager;
    explicit AudioManager();

public:
    std::shared_ptr<AudioListener> audioListener;
    std::map<int, std::shared_ptr<AudioSource>> audioSources;
    std::shared_ptr<ALCdevice*> audioDevice;
    std::shared_ptr<ALCcontext*> audioContext;

    AudioManager(AudioManager &other) = delete;
    void operator=(const AudioManager&) = delete;
    virtual ~AudioManager();

    static AudioManager* GetInstance();

    void InitializeAudio();
    void RemoveAudioSource(int componentId);

    void Free();

};

#endif //GLOOMENGINE_AUDIOMANAGER_H
