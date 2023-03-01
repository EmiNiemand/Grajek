#ifndef OPENGLGP_GLOOMENGINE_H
#define OPENGLGP_GLOOMENGINE_H

#include "LowLevelClasses/Shader.h"
#include "HighLevelClasses/GameObject.h"

class GloomEngine {
private:
    int* width;
    int* height;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    std::map<ComponentNames,std::shared_ptr<Component>> components;
    std::map<std::string, std::shared_ptr<GameObject>> gameObjects;

    //TODO: Move it to rendering component later
    Shader shader = Shader("res/shaders/basic.vert", "res/shaders/basic.frag");

public:
    GloomEngine(int *width, int *height);
    virtual ~GloomEngine();

    void Awake();
    void Start();
    void Update(float currentFrame);
    void End();

    //Other engine methods
    float GetDeltaTime() const;
    float GetLastFrame() const;
};


#endif //OPENGLGP_GLOOMENGINE_H
