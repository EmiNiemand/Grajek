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
    float mass = 1;
    float linearDrag = 10;
    float gravityScale = 5;

public:
    Rigidbody(const std::shared_ptr<GameObject> &parent, int id);
    ~Rigidbody() override;

    void Update() override;

    void AddForce(glm::vec3 vector, ForceMode forceMode);
};


#endif //OPENGLGP_RIGIDBODY_H
