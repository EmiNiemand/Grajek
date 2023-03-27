#include "Components/Renderers/Camera.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"


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

void Camera::SetZoomLevel(float newzoom)
{
    if(newzoom > 0.01f)
    desiredZoomLevel = newzoom;
}

void Camera::Start() {
    player = GloomEngine::GetInstance()->FindGameObjectWithName("Player");
    glm::vec3 playerPosition = player->transform->GetLocalPosition();
    parent->transform->SetLocalPosition(playerPosition + cameraOffset * zoomLevel);
    parent->transform->SetLocalRotation(glm::vec3(-45.0f, 0.0f, 0.0f));
    Component::Start();
}

void Camera::Update() {
    // we change desired zoom on the camera when pressing Z. 
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_Z)) {
        if (!isZoomed) {
            isZoomed = true;
            SetZoomLevel(0.5f);
        } else {
            isZoomed = false;
            SetZoomLevel(1.0f);
        }
    }
    glm::vec3 playerPosition = player->transform->GetLocalPosition();
    glm::vec3 cameraPosition = parent->transform->GetLocalPosition();
    zoomLevel = Lerp(zoomLevel, desiredZoomLevel, parameter);
    parent->transform->SetLocalPosition(Lerp(parent->transform->GetGlobalPosition(), playerPosition + cameraOffset * zoomLevel, parameter));
    Component::Update();
}

glm::vec3 Camera::Lerp(glm::vec3 firstPoint, glm::vec3 secondPoint, float parameter) {
    return firstPoint + (secondPoint - firstPoint) * parameter;
}
float Camera::Lerp(float scalar1,float scalar2,float parameter) {
    return scalar1 + (scalar2 - scalar1) * parameter;
}
