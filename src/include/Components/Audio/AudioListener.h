//
// Created by Adrian on 07.04.2023.
//

#ifndef GLOOMENGINE_AUDIOLISTENER_H
#define GLOOMENGINE_AUDIOLISTENER_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>
#include <al.h>

class AudioListener {
    glm::vec3 position = {};
    glm::vec3 velocity = {};
    float gain = 0.5f;

public:
    explicit AudioListener();
    ~AudioListener();

    static void UpdatePosition(const glm::vec3& position);
    void SetGain(const float& val);
    [[nodiscard]] const float GetGain() const;
    void SetVelocity(const glm::vec3& vel);

};

#endif //GLOOMENGINE_AUDIOLISTENER_H
