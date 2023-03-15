#ifndef OPENGLGP_CAMERA_H
#define OPENGLGP_CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "include/Components/Component.h"

class Camera : public Component {
private:
    std::shared_ptr<GameObject> target;

public:
    Camera(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id);
    ~Camera() override;

    void SetTarget(const std::shared_ptr<GameObject> &target);

    glm::mat4 GetViewMatrix();
};


#endif //OPENGLGP_CAMERA_H
