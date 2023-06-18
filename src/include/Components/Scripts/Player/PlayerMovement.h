//
// Created by szymo on 16/03/2023.
//

#ifndef OPENGLGP_PLAYERMOVEMENT_H
#define OPENGLGP_PLAYERMOVEMENT_H

#include "Components/Component.h"
#include "glm/vec2.hpp"

class Rigidbody;
class AudioSource;

class PlayerMovement : public Component {
private:
    std::shared_ptr<Rigidbody> rb;
    glm::vec2 moveVector = {};
    float speed = 0.0f;
    float maxSpeed = 0.2f;
    float smoothingParam = 0.2f;
    float rotationAngle = 0.0f;

    bool isMoving = false;

public:
    PlayerMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~PlayerMovement() override;

    void Start() override;

    void OnDestroy() override;

    void FixedUpdate() override;
    void Move(glm::vec2 moveVector);

};


#endif //OPENGLGP_PLAYERMOVEMENT_H
