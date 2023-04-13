//
// Created by Adrian on 07.04.2023.
//

#ifndef GLOOMENGINE_AUDIOLOADER_H
#define GLOOMENGINE_AUDIOLOADER_H

#include <al.h>
#include <vector>
#include <memory>
#include <string>

enum class AudioFileType {
    mp3,
    wav,
};

class AudioLoader {
     void LoadWav(const char*);
     void LoadMp3(const char*);

public:
    int sampleRate;
    int bitsPerSample;
    ALenum format;
    std::vector<int> soundData;

    explicit AudioLoader();
    virtual ~AudioLoader();

    void LoadAudioFile(const char*, AudioFileType type = AudioFileType::wav);
};



#endif //GLOOMENGINE_AUDIOLOADER_H
