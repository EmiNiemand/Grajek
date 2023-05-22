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

        newPosition = glm::normalize((*path)[pathIterator] - currentPosition) * speed * speedMultiplier;

        rigidbody->AddForce(newPosition, ForceMode::Force);

        rotationAngle = std::atan2f(-newPosition.x, -newPosition.z) * 180.0f / std::numbers::pi;

        if (rotationAngle < 0.0f) {
            rotationAngle += 360.0f;
        }

        rigidbody->AddTorque(rotationAngle, ForceMode::Force);

        if (glm::distance(currentPosition, (*path)[pathIterator]) < 0.75f * AIManager::GetInstance()->aiCellSize)
            --pathIterator;
    }

    Component::FixedUpdate();
}

void CharacterMovement::AIUpdate() {
    currentPosition = parent->transform->GetGlobalPosition();

    if (endPointsIterator < 0 && logicState != RunningToPlayer) {
        SetRandomEndPoint();
    }

//    if (pathIterator < 1 && logicState != RunningToPlayer) {
//        CalculatePath();
//        --endPointsIterator;
//    }

    if (pathIterator < 0 && logicState != RunningToPlayer) {
        CalculatePath();
        --endPointsIterator;
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
    path->clear();
    rigidbody = nullptr;
    pathfinding = nullptr;
    Component::OnDestroy();
}

void CharacterMovement::Free() {
    path->clear();
    rigidbody = nullptr;
    pathfinding = nullptr;
}

void CharacterMovement::SetRandomEndPoint() {
    speed = 0.0f;

    static glm::ivec2 newEndPoint;

    const int aiCellSize = (int)std::ceil(AIManager::GetInstance()->aiCellSize);
    const int aiGridSize = AI_GRID_SIZE / 4 - 1;

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(-aiGridSize / aiCellSize, aiGridSize / aiCellSize);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(-aiGridSize / aiCellSize, aiGridSize / aiCellSize);

        if (!AIManager::GetInstance()->aiGrid[newEndPoint.x + AI_GRID_SIZE / 2][newEndPoint.y + AI_GRID_SIZE / 2])
            break;
    }

    endPoint = {newEndPoint.x, 0, newEndPoint.y};

    SetSubEndPoints();
}

const glm::vec3 CharacterMovement::GetNewSpawnPoint() {
    static int x, z;

    const int aiCellSize = (int)std::ceil(AIManager::GetInstance()->aiCellSize);
    const int aiGridSize = AI_GRID_SIZE / 4 - 1;

    while (true) {
        x = RandomnessManager::GetInstance()->GetInt(-aiGridSize / aiCellSize, aiGridSize / aiCellSize);
        z = RandomnessManager::GetInstance()->GetInt(-aiGridSize / aiCellSize, aiGridSize / aiCellSize);

        if (!AIManager::GetInstance()->aiGrid[x + AI_GRID_SIZE / 2][z + AI_GRID_SIZE / 2])
            break;
    }

    return glm::vec3(x, 0, z);
}

void CharacterMovement::SetNewPath(AI_LOGICSTATE state) {
    logicState = state;

    if (state == RunningToPlayer) {
        previousEndPoint = endPoint;

        static glm::vec3 playerPos = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform->GetLocalPosition();
        static int x = (int)playerPos.x, z = (int)playerPos.z;
        static bool isAvailable = false;

        for (int i = -2; i <= 2; ++i) {
            for (int j = -2; j <= 2; ++j) {
                if (i == 0 && j == 0)
                    continue;

                if (!AIManager::GetInstance()->aiGrid[x + i + AI_GRID_SIZE / 2][z + j + AI_GRID_SIZE / 2]) {
                    endPoint = {x + i, 0, z + j};
                    isAvailable = true;
                    break;
                }
            }

            if (isAvailable)
                break;
        }

        SetSubEndPoints();

        speedMultiplier = 2.0f;
    } else if (state == WalkingOnPath) {
        endPoint = previousEndPoint;
        speedMultiplier = 1.0f;
    }

    CalculatePath();
}

void CharacterMovement::SetSubEndPoints() {
    subEndPoints.clear();
    subEndPoints.reserve(4);

    static glm::ivec2 newEndPoint;
    static float multiplier = 0.25f;
    static bool isAvailable = false;

    while (multiplier < 0.9f) {
        newEndPoint = {endPoint.x, endPoint.y};
        newEndPoint *= multiplier;

        if (AIManager::GetInstance()->aiGrid[newEndPoint.x + AI_GRID_SIZE / 2][newEndPoint.y + AI_GRID_SIZE / 2]) {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0)
                        continue;

                    if (!AIManager::GetInstance()->aiGrid[newEndPoint.x + i + AI_GRID_SIZE / 2][newEndPoint.y + j + AI_GRID_SIZE / 2]) {
                        subEndPoints.emplace_back(newEndPoint.x + i, 0, newEndPoint.y + j );
                        isAvailable = true;
                        break;
                    }
                }

                if (isAvailable)
                    break;
            }
        } else {
            subEndPoints.emplace_back(endPoint * multiplier);
        }

        isAvailable = false;
        multiplier += 0.25f;
    }

    subEndPoints.push_back(endPoint);

    endPointsIterator = 3;
}

void CharacterMovement::CalculatePath() {
#ifdef DEBUG
    ZoneScopedNC("CalculatePath", 0xfc0f03);
#endif

    delete path;

    path = pathfinding->FindNewPath({currentPosition.x, currentPosition.z},
                                    {endPoint.x, endPoint.z});

    if (path == nullptr) {
        pathIterator = -1;
        return;
    }

    pathIterator = path->size() - 1;
}
