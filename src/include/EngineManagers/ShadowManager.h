#ifndef GLOOMENGINE_SHADOWMANAGER_H
#define GLOOMENGINE_SHADOWMANAGER_H

#include <memory>

class Shader;

class ShadowManager {
private:
    inline static ShadowManager* shadowManager;

public:
    std::shared_ptr<Shader> shadowShader;
    unsigned int depthMapFBO;
    unsigned int depthMap;

    unsigned int shadowResolution = 2048;
    float nearPlane = 1.0f, farPlane = 60.0f;

public:
    ShadowManager(ShadowManager &other) = delete;
    void operator=(const ShadowManager&) = delete;
    virtual ~ShadowManager();

    static ShadowManager* GetInstance();

    void PrepareShadow();

private:
    ShadowManager();
};


#endif //GLOOMENGINE_SHADOWMANAGER_H
