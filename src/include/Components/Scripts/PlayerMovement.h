//
// Created by szymo on 16/03/2023.
//

#ifndef OPENGLGP_PLAYERMOVEMENT_H
#define OPENGLGP_PLAYERMOVEMENT_H

#include "Components/Component.h"
#include "glm/vec2.hpp"

class Rigidbody;

class PlayerMovement : public Component {
private:
    float speed = 0.0f;
    float maxSpeed = 1.5f;
    float smoothingParam = 0.5f;
    bool isMoving = false;
    float horizontal = 0.0f;
    float vertical = 0.0f;
    float rotationAngle = 0.0f;
    std::shared_ptr<Rigidbody> rb;
	glm::vec2 moveVector;

public:
    PlayerMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~PlayerMovement() override;

    void Start() override;
    void FixedUpdate() override;
    void Move(glm::vec2 moveVector);
};


#endif //OPENGLGP_PLAYERMOVEMENT_H
