//
// Created by szymo on 02/03/2023.
//

#include "include/Components/Camera.h"
#include "include/HighLevelClasses/GameObject.h"

Camera::Camera(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent) : Component(
        gloomEngine, parent) {
    name = ComponentNames::CAMERA;
}


glm::mat4 Camera::GetViewMatrix() {

    glm::vec3 position = parent->transform.GetGlobalPosition();
    glm::vec3 front = parent->transform.GetForward();
    glm::vec3 up = parent->transform.GetUp();

    if(target == nullptr) {
        return glm::lookAt(position, position + front, up);
    }
    else {
        glm::vec3 targetPosition = target->transform.GetGlobalPosition();
        return glm::lookAt(position, targetPosition - position, up);
    }
}
