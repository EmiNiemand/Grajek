#include "Other/FrustumCulling.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Camera.h"
#include "LowLevelClasses/Model.h"
#include "EngineManagers/OptionsManager.h"

FrustumCulling::FrustumCulling() = default;

FrustumCulling::~FrustumCulling() = default;

FrustumCulling *FrustumCulling::GetInstance() {
    if (frustumCulling == nullptr) {
        frustumCulling = new FrustumCulling();
    }
    return frustumCulling;
}

void FrustumCulling::UpdateFrustum() {
    std::shared_ptr<GameObject> activeCamera = Camera::activeCamera;
    glm::vec3 camPosition = activeCamera->transform->GetGlobalPosition();
    glm::vec3 camFront = activeCamera->transform->GetForward();
    glm::vec3 camUp = activeCamera->transform->GetUp();
    glm::vec3 camRight = activeCamera->transform->GetRight();

    float aspect = (float)OptionsManager::GetInstance()->width / (float)OptionsManager::GetInstance()->height;

    float fov = RendererManager::GetInstance()->fov;
    float zNear = RendererManager::GetInstance()->zNear;
    float zFar = RendererManager::GetInstance()->zFar;

    const float halfVSide = zFar * tanf(fov * .5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = zFar * camFront;

    frustum.nearFace = { camPosition + zNear * camFront, camFront };
    frustum.farFace = { camPosition + frontMultFar, -camFront };
    frustum.rightFace = { camPosition, glm::cross(frontMultFar - camRight * halfHSide, camUp) };
    frustum.leftFace = { camPosition, glm::cross(camUp, frontMultFar + camRight * halfHSide) };
    frustum.topFace = { camPosition, glm::cross(camRight, frontMultFar - camUp * halfVSide) };
    frustum.bottomFace = { camPosition, glm::cross(frontMultFar + camUp * halfVSide, camRight) };
}

bool FrustumCulling::IsOnFrustum(const std::shared_ptr<AABB> &bounds, const std::shared_ptr<Transform> &transform) {
    return bounds->IsOnFrustum(frustum, transform);
}

std::shared_ptr<AABB> FrustumCulling::GenerateAABB(const std::shared_ptr<Model> &model) {
    if (!model) {
        return std::make_shared<AABB>(glm::vec3(0.001f), glm::vec3(0.001f));
    }
    auto minAABB = glm::vec3(std::numeric_limits<float>::max());
    auto maxAABB = glm::vec3(std::numeric_limits<float>::min());
    for (auto&& mesh : model->GetMeshes())
    {
        for (auto&& vertex : mesh.vertices)
        {
            minAABB.x = std::min(minAABB.x, vertex.position.x);
            minAABB.y = std::min(minAABB.y, vertex.position.y);
            minAABB.z = std::min(minAABB.z, vertex.position.z);

            maxAABB.x = std::max(maxAABB.x, vertex.position.x);
            maxAABB.y = std::max(maxAABB.y, vertex.position.y);
            maxAABB.z = std::max(maxAABB.z, vertex.position.z);
        }
    }
    return std::make_shared<AABB>(minAABB, maxAABB);
}

