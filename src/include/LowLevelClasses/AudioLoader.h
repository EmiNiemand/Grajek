//
// Created by Adrian on 07.04.2023.
//

#ifndef GLOOMENGINE_AUDIOLOADER_H
#define GLOOMENGINE_AUDIOLOADER_H

#include "ProjectSettings.h"
#include <vector>
#include <al.h>

class AudioLoader {

public:
    explicit AudioLoader();
    virtual ~AudioLoader();

    static void LoadAudioFile(const char* path, AudioType type, ALuint bufferId);
};



#endif //GLOOMENGINE_AUDIOLOADER_H
