//
// Created by Adrian on 07.04.2023.
//

#ifndef GLOOMENGINE_AUDIOLISTENER_H
#define GLOOMENGINE_AUDIOLISTENER_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include <al.h>

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

    void SetGain(const float& val);
    const float GetGain() const;
    void SetVelocity(const glm::vec3& vel);

};

#endif //GLOOMENGINE_AUDIOLISTENER_H
