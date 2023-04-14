//
// Created by szymo on 16/03/2023.
//

#ifndef OPENGLGP_RIGIDBODY_H
#define OPENGLGP_RIGIDBODY_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ProjectSettings.h"
#include "Components/Component.h"

class Rigidbody : public Component {
public:
    glm::vec3 velocity = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0}; // in degrees
    float mass = 1;
    float linearDrag = 10;
    float turnSpeed = 7.5f;
    float gravityScale = 5;

public:
    Rigidbody(const std::shared_ptr<GameObject> &parent, int id);
    ~Rigidbody() override;

    void FixedUpdate() override;

    void AddForce(glm::vec3 vector, ForceMode forceMode);
    void AddTorque(float angle, ForceMode forceMode);
};


#endif //OPENGLGP_RIGIDBODY_H
