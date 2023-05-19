//
// Created by Adrian on 15.05.2023.
//

#ifndef GLOOMENGINE_INDICATOR_H
#define GLOOMENGINE_INDICATOR_H

#include "Components/Component.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class GameObject;
class Image;

class Indicator : public Component {
    float x, y;
    const char* texturePath;
    std::shared_ptr<GameObject> camera;

public:
    Indicator(const std::shared_ptr<GameObject> &parent, int id);
    ~Indicator() override;

    void OnCreate() override;
    void SetTexturePath(const char* path);
    void Indicate();

};

#endif //GLOOMENGINE_INDICATOR_H
