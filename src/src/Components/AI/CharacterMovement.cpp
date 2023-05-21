//
// Created by Adrian on 01.05.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/AI/CharacterStates.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include <numbers>

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterMovement::CharacterMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterMovement::~CharacterMovement() = default;

void CharacterMovement::FixedUpdate() {
#ifdef DEBUG
    ZoneScopedNC("CharacterMovement", 0xfc0f03);
#endif

    currentPosition = parent->transform->GetLocalPosition();

    if (pathIterator >= 0) {
        speed = std::lerp(speed, maxSpeed, smoothingParam);

        newPosition = glm::normalize(path[pathIterator] - currentPosition) * speed * speedMultiplier;

        rigidbody->AddForce(newPosition, ForceMode::Force);

        rotationAngle = std::atan2f(-newPosition.x, -newPosition.z) * 180.0f / std::numbers::pi;

        if (rotationAngle < 0.0f) {
            rotationAngle += 360.0f;
        }

        rigidbody->AddTorque(rotationAngle, ForceMode::Force);

        if (glm::distance(currentPosition, path[pathIterator]) < 0.5f)
            --pathIterator;
    }

    Component::FixedUpdate();
}

void CharacterMovement::AIUpdate() {
    if (pathIterator < 0 && logicState != RunningToPlayer) {
        currentPosition = parent->transform->GetGlobalPosition();
        SetNewRandomEndPoint();
        CalculateNewPath();
    }

    Component::AIUpdate();
}

void CharacterMovement::OnCreate() {
    rigidbody = parent->GetComponent<Rigidbody>();
    pathfinding = AIManager::GetInstance()->pathfinding;
    parent->transform->SetLocalPosition(GetNewSpawnPoint());
    Component::OnCreate();
}

void CharacterMovement::OnDestroy() {
    path.clear();
    rigidbody = nullptr;
    Component::OnDestroy();
}

void CharacterMovement::Free() {
    path.clear();
    rigidbody = nullptr;
}

void CharacterMovement::SetNewRandomEndPoint() {
    speed = 0.0f;

    static glm::ivec2 newEndTarget;

    while (true) {
        newEndTarget.x = RandomnessManager::GetInstance()->GetInt(-24, 24);
        newEndTarget.y = RandomnessManager::GetInstance()->GetInt(-24, 24);

        if (!AIManager::GetInstance()->aiGrid[newEndTarget.x + AI_GRID_SIZE / 2][newEndTarget.y + AI_GRID_SIZE / 2])
            break;
    }

    endTarget = {newEndTarget.x, 0, newEndTarget.y};
}

const glm::vec3 CharacterMovement::GetNewSpawnPoint() {
    int x, z;

    while (true) {
        x = RandomnessManager::GetInstance()->GetInt(-24, 24);
        z = RandomnessManager::GetInstance()->GetInt(-24, 24);

        if (!AIManager::GetInstance()->aiGrid[x + AI_GRID_SIZE / 2][z + AI_GRID_SIZE / 2])
            break;
    }

    return glm::vec3(x, 0, z);
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

    path = pathfinding->FindNewPath({currentPosition.x, currentPosition.z},
                                    {endTarget.x, endTarget.z});

    pathIterator = path.size() - 1;
}
