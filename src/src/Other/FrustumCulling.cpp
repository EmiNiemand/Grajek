#include "Other/FrustumCulling.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Camera.h"
#include "LowLevelClasses/Model.h"
#include "EngineManagers/OptionsManager.h"
#include "EngineManagers/ShadowManager.h"

FrustumCulling::FrustumCulling() = default;

FrustumCulling::~FrustumCulling() {
    delete frustumCulling;
}

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

    const float halfWSide = zFar * tanf(fov * .5f);
    const float halfHSide = halfWSide * aspect;
    const glm::vec3 frontMultFar = zFar * camFront;

    frustum.nearFace = { camPosition + zNear * camFront, camFront };
    frustum.farFace = { camPosition + frontMultFar, -camFront };
    frustum.rightFace = { camPosition, glm::cross(frontMultFar - camRight * halfHSide, camUp) };
    frustum.leftFace = { camPosition, glm::cross(camUp, frontMultFar + camRight * halfHSide) };
    frustum.topFace = { camPosition, glm::cross(camRight, frontMultFar - camUp * halfWSide) };
    frustum.bottomFace = { camPosition, glm::cross(frontMultFar + camUp * halfWSide, camRight) };


    std::shared_ptr<GameObject> sun = GloomEngine::GetInstance()->FindGameObjectWithName("Sun");
    if (!sun) return;

    glm::vec3 playerPosition = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform->GetGlobalPosition();
    playerPosition.y = 0;
    glm::vec3 sunPosition = sun->transform->GetGlobalPosition() + playerPosition;
    glm::vec3 sunFront = sun->transform->GetForward();
    glm::vec3 sunUp = sun->transform->GetUp();
    glm::vec3 sunRight = sun->transform->GetRight();

    float sunZNear = ShadowManager::GetInstance()->nearPlane;
    float sunZFar = ShadowManager::GetInstance()->farPlane;

    const float sunHalfWSide = activeCamera->transform->GetGlobalPosition().y;
    const float sunHalfHSide = sunHalfWSide;
    const glm::vec3 sunFrontMultFar = sunZFar * sunFront;

    shadowFrustum.nearFace = { sunPosition + sunZNear * sunFront, sunFront };
    shadowFrustum.farFace = { sunPosition + sunFrontMultFar, -sunFront };
    shadowFrustum.rightFace = { sunPosition + sunZNear * sunFront + sunRight * sunHalfWSide , -sunRight };
    shadowFrustum.leftFace = { sunPosition - sunZNear * sunFront - sunRight * sunHalfWSide, sunRight };
    shadowFrustum.topFace = { sunPosition + sunZNear * sunFront + sunUp * sunHalfHSide, -sunUp };
    shadowFrustum.bottomFace = { sunPosition - sunZNear * sunFront - sunUp * sunHalfHSide, sunUp };
}

bool FrustumCulling::IsOnFrustum(const std::shared_ptr<AABB> &bounds, const std::shared_ptr<Transform> &transform) {
    return bounds->IsOnFrustum(frustum, transform) || bounds->IsOnFrustum(shadowFrustum, transform);
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

