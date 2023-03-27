#ifndef OPENGLGP_CAMERA_H
#define OPENGLGP_CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"

class Camera : public Component {
private:
    std::shared_ptr<GameObject> target;
    std::shared_ptr<GameObject> player;

public:
    static std::shared_ptr<GameObject> activeCamera;

    glm::vec3 cameraOffset = glm::vec3(0,0,0);

public:
    Camera(const std::shared_ptr<GameObject> &parent, int id);
    ~Camera() override;

    void Start() override;
    void Update() override;

    void SetTarget(const std::shared_ptr<GameObject> &target);

    glm::mat4 GetViewMatrix();
};


#endif //OPENGLGP_CAMERA_H
