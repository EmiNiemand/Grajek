#include "include/GloomEngine.h"

GloomEngine::GloomEngine(int *width, int *height) : width(width), height(height) {
}

GloomEngine::~GloomEngine() {

}

void GloomEngine::Awake() {
    for (auto&& component : components){
        component.second->Awake();
    }
}

void GloomEngine::Start() {
    for (auto&& component : components){
        component.second->Start();
    }
}

void GloomEngine::Update(float currentFrame) {
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    for (auto&& component : components){
        component.second->Update();
    }
}

void GloomEngine::End() {
    components.clear();
    gameObjects.clear();
    shader.Delete();
}

// Other

float GloomEngine::GetDeltaTime() const {
    return deltaTime;
}

float GloomEngine::GetLastFrame() const {
    return lastFrame;
}
