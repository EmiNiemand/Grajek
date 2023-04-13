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
    std::shared_ptr<GameObject> player;

public:
    AudioListener(const std::shared_ptr<GameObject> &parent, int id);
    ~AudioListener() override;

    void Start() override;
    void Update() override;
    void OnCreate() override;


};

#endif //GLOOMENGINE_AUDIOLISTENER_H
