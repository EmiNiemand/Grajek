#ifndef GLOOMENGINE_OPTIONSMANAGER_H
#define GLOOMENGINE_OPTIONSMANAGER_H

#include <cstdint>

class OptionsManager {
private:
    inline static OptionsManager* optionsManager;

public:
    float musicVolume;
    int32_t width;
    int32_t height;
    bool fullScreen;
    unsigned int shadowResolution;

    bool sessionMetronomeVisuals;
    bool sessionMetronomeSound;
    bool sessionBackingTrack;

public:
    OptionsManager(OptionsManager &other) = delete;
    void operator=(const OptionsManager&) = delete;
    virtual ~OptionsManager();

    static OptionsManager* GetInstance();

    void Load();
    void Save();

private:
    explicit OptionsManager();
};

#endif //GLOOMENGINE_OPTIONSMANAGER_H