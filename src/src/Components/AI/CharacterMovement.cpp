//
// Created by Adrian on 01.05.2023.
//

#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"

#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/AI/CharacterStates.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/AI/CharacterPathfinding.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include <numbers>

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterMovement::CharacterMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterMovement::~CharacterMovement() = default;

void CharacterMovement::Start() {
    SetNewRandomPoint();
    parent->transform->SetLocalPosition(endTarget);
    currentPosition = endTarget;
    Component::Start();
}

void CharacterMovement::FixedUpdate() {
#ifdef DEBUG
    ZoneScopedNC("CharacterMovement", 0xfc0f03);
#endif

    currentPosition = parent->transform->GetLocalPosition();

    if (!path->empty()) {
        speed = std::lerp(speed, maxSpeed, smoothingParam);

        newPosition = glm::normalize((*path)[0] - currentPosition) * speed * speedMultiplier;

        rigidbody->AddForce(newPosition, ForceMode::Force);

        rotationAngle = std::atan2f(-newPosition.x, -newPosition.z) * 180.0f / std::numbers::pi;

        if (rotationAngle < 0.0f) {
            rotationAngle += 360.0f;
        }

        rigidbody->AddTorque(rotationAngle, ForceMode::Force);

        if (glm::distance(currentPosition, (*path)[0]) < 0.6f)
            path->erase(path->begin());
    }

    Component::FixedUpdate();
}

void CharacterMovement::AIUpdate() {
    if (path->empty() && logicState != RunningToPlayer) {
        currentPosition = parent->transform->GetGlobalPosition();
        SetNewRandomPoint();
        CalculateNewPath();
    }

    Component::AIUpdate();
}

void CharacterMovement::OnCreate() {
    rigidbody = parent->GetComponent<Rigidbody>();
    path = new std::vector<glm::vec3> (1);
    Component::OnCreate();
}

void CharacterMovement::OnDestroy() {
    rigidbody = nullptr;
    path->clear();
    Component::OnDestroy();
}

void CharacterMovement::Free() {
    rigidbody = nullptr;
    path->clear();
}

void CharacterMovement::SetNewRandomPoint() {
    speed = 0.0f;

    static glm::ivec2 newEndTarget;

    while (true) {
        newEndTarget.x = RandomnessManager::GetInstance()->GetInt(-20, 20);
        newEndTarget.y = RandomnessManager::GetInstance()->GetInt(-20, 20);

        if (!AIManager::GetInstance()->pathfinding->aiGrid[newEndTarget.x + AI_GRID_SIZE / 2][newEndTarget.y + AI_GRID_SIZE / 2])
            break;
    }
    spdlog::info("end " + std::to_string(newEndTarget.x) + ", " + std::to_string(newEndTarget.y));

    endTarget.x = (float)newEndTarget.x;
    endTarget.z = (float)newEndTarget.y;
}

void CharacterMovement::SetNewPath(AI_LOGICSTATE state) {
    logicState = state;

    if (state == RunningToPlayer) {
        previousTarget = endTarget;
        endTarget = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform->GetLocalPosition();
        endTarget.x -= RandomnessManager::GetInstance()->GetFloat(0.5f, 2.0f);
        endTarget.z -= RandomnessManager::GetInstance()->GetFloat(0.5f, 2.0f);
        speedMultiplier = 2.0f;
    } else if (state == WalkingOnPath) {
        endTarget = previousTarget;
        speedMultiplier = 1.0f;
    }

    CalculateNewPath();
}

void CharacterMovement::CalculateNewPath() {
#ifdef DEBUG
    ZoneScopedNC("CalculateNewPath", 0xfc0f03);
#endif
    
    delete path;

    path = AIManager::GetInstance()->pathfinding->FindNewPath({
        currentPosition.x, currentPosition.z},{endTarget.x, endTarget.z});
}
