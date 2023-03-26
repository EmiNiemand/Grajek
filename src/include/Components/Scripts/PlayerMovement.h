//
// Created by szymo on 16/03/2023.
//

#ifndef OPENGLGP_PLAYERMOVEMENT_H
#define OPENGLGP_PLAYERMOVEMENT_H

#include "Components/Component.h"

class Rigidbody;

class PlayerMovement : public Component {
private:
    float speed = 0.01;
    std::shared_ptr<Rigidbody> rb;

public:
    PlayerMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~PlayerMovement() override;

    void Start() override;
    void Update() override;
};


#endif //OPENGLGP_PLAYERMOVEMENT_H
