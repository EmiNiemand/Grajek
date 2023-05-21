#include "Components/Renderers/Camera.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Utilities.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

Camera::Camera(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Camera::~Camera() = default;

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
    RendererManager::GetInstance()->UpdateCamera();
}

void Camera::SetZoomLevel(float newZoom)
{
    if(newZoom > 0.01f) desiredZoomLevel = newZoom;
    RendererManager::GetInstance()->UpdateCamera();
}

void Camera::Start() {
    player = GloomEngine::GetInstance()->FindGameObjectWithName("PlayerPivot");
    glm::vec3 playerPosition = player->transform->GetGlobalPosition();
    parent->transform->SetLocalPosition(playerPosition + cameraOffset * zoomLevel);
    parent->transform->SetLocalRotation(glm::vec3(-45.0f, 0.0f, 0.0f));
    Component::Start();
}

void Camera::Update() {
#ifdef DEBUG
    ZoneScopedNC("Camera", 0x800080);
#endif
    if (GloomEngine::GetInstance()->timeScale == 1) {
        glm::vec3 playerPosition = player->transform->GetGlobalPosition();
        glm::vec3 cameraPosition = parent->transform->GetLocalPosition();
        zoomLevel = Utilities::Lerp(zoomLevel, desiredZoomLevel, 0.02f);
        parent->transform->SetLocalPosition(Utilities::Lerp(
                parent->transform->GetGlobalPosition(),
                playerPosition + cameraOffset * zoomLevel, 0.02f));
    }

    Component::Update();
}

void Camera::OnUpdate() {
    RendererManager::GetInstance()->UpdateCamera();
    Component::OnUpdate();
}
