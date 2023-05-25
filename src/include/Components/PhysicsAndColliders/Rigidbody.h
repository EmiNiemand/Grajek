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
    /// if drag > gravity bug that launches player into space can occur
    float linearDrag = 5;
    float turnSpeed = 5.0f;
    float gravityScale = 5;

public:
    Rigidbody(const std::shared_ptr<GameObject> &parent, int id);
    ~Rigidbody() override;

    void FixedUpdate() override;

    void AddForce(const glm::vec3 &vector, ForceMode forceMode);
    void AddTorque(float targetRotation, ForceMode forceMode);

};


#endif //OPENGLGP_RIGIDBODY_H
