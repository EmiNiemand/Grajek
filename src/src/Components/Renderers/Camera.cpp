#include "include/Components/Renderers/Camera.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/GameObjectsAndPrefabs/GameObject.h"

Camera::Camera(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id) : Component(
        gloomEngine, parent, id) {}

Camera::~Camera() {}



glm::mat4 Camera::GetViewMatrix() {
    glm::vec3 position = parent->transform->GetGlobalPosition();
    glm::vec3 front = parent->transform->GetForward();
    glm::vec3 up = parent->transform->GetUp();

    if(target == nullptr) {
        return glm::lookAt(position, position + front, up);
    }
    else {
        glm::vec3 targetPosition = target->transform->GetGlobalPosition();
        return glm::lookAt(position, targetPosition, up);
    }
}

void Camera::SetTarget(const std::shared_ptr<GameObject> &target) {
    Camera::target = target;
}
