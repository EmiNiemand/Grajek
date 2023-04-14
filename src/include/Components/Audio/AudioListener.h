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

public:
    AudioListener(const std::shared_ptr<GameObject> &parent, int id);
    ~AudioListener() override;

    void Start() override;
    void Update() override;
    void OnCreate() override;

    void SetGain(float val) const;
    void SetVelocity(glm::vec3 velocity) const;
};

#endif //GLOOMENGINE_AUDIOLISTENER_H
