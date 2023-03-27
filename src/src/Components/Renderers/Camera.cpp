#include "Components/Renderers/Camera.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Utilities.h"

std::shared_ptr<GameObject> Camera::activeCamera = nullptr;

Camera::Camera(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

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
    player = GloomEngine::GetInstance()->FindGameObjectWithName("Player");
    glm::vec3 playerPosition = player->transform->GetLocalPosition();
    parent->transform->SetLocalPosition(glm::vec3(playerPosition.x + cameraOffset.x, playerPosition.y + cameraOffset.y, playerPosition.z + cameraOffset.z));
    parent->transform->SetLocalRotation(glm::vec3(-45.0f, 0.0f, 0.0f));
    Component::Start();
}

void Camera::Update() {
    glm::vec3 playerPosition = player->transform->GetLocalPosition();
    glm::vec3 cameraPosition = parent->transform->GetLocalPosition();
    parent->transform->SetLocalPosition(Utilities::GetInstance()->Lerp(parent->transform->GetGlobalPosition(), glm::vec3(playerPosition.x + cameraOffset.x, playerPosition.y + cameraOffset.y, playerPosition.z + cameraOffset.z), 0.02f));
    Component::Update();
}
