//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AUDIOSOURCE_H
#define GLOOMENGINE_AUDIOSOURCE_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include <vector>
#include <unordered_map>

class GameObject;

class AudioSource : public Component {
    glm::vec3 position = {};

public:
    AudioSource(const std::shared_ptr<GameObject> &parent, int id);
    ~AudioSource() override;

    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;


};

#endif //GLOOMENGINE_AUDIOSOURCE_H
