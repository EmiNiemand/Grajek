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
    float speed = 0.01;
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
