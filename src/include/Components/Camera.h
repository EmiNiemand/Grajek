//
// Created by szymo on 02/03/2023.
//

#ifndef OPENGLGP_CAMERA_H
#define OPENGLGP_CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Component.h"

class Camera : public Component {
private:
    std::shared_ptr<GameObject> target;

public:
    Camera(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent);

    glm::mat4 GetViewMatrix();
};


#endif //OPENGLGP_CAMERA_H
