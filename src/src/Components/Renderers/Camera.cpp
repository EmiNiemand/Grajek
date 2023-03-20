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

void Camera::Start() {
    player = gloomEngine->FindGameObjectWithName("Player");
    glm::vec3 playerPosition = player->transform->GetLocalPosition();
    parent->transform->SetLocalPosition(glm::vec3(playerPosition.x + cameraOffset.x, playerPosition.y + cameraOffset.y, playerPosition.z + cameraOffset.z));
    parent->transform->SetLocalRotation(glm::vec3(-45.0f, 0.0f, 0.0f));
    Component::Start();
}

void Camera::Update() {
    glm::vec3 playerPosition = player->transform->GetLocalPosition();
    glm::vec3 cameraPosition = parent->transform->GetLocalPosition();
    spdlog::info("POS: "+std::to_string(cameraPosition.x)+", "+std::to_string(cameraPosition.y)+", "+std::to_string(cameraPosition.z));
    parent->transform->SetLocalPosition(Lerp(parent->transform->GetGlobalPosition(), glm::vec3(playerPosition.x + cameraOffset.x, playerPosition.y + cameraOffset.y, playerPosition.z + cameraOffset.z), parameter));
    spdlog::info(std::to_string(cameraPosition.x)+", "+std::to_string(cameraPosition.y)+", "+std::to_string(cameraPosition.z));
    Component::Update();
}

glm::vec3 Camera::Lerp(glm::vec3 firstPoint, glm::vec3 secondPoint, float parameter) {
    return firstPoint + (secondPoint - firstPoint) * parameter;
}
