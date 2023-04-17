//
// Created by Adrian on 07.04.2023.
//

#ifndef GLOOMENGINE_AUDIOLISTENER_H
#define GLOOMENGINE_AUDIOLISTENER_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include <vector>
#include <unordered_map>

class GameObject;

class AudioListener : public Component {
    glm::vec3 position = {};
    glm::vec3 velocity = {};
    float gain = 1.0f;

public:
    AudioListener(const std::shared_ptr<GameObject> &parent, int id);
    ~AudioListener() override;

    void Start() override;
    void Update() override;
    void OnCreate() override;

    void SetGain(float val);
    void SetVelocity(glm::vec3 velocity);

    float GetGain();
};

#endif //GLOOMENGINE_AUDIOLISTENER_H