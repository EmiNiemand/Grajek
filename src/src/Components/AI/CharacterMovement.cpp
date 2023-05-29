//
// Created by Adrian on 01.05.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "EngineManagers/CollisionManager.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include <numbers>

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterMovement::CharacterMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterMovement::~CharacterMovement() = default;

void CharacterMovement::Start() {
    collisionGrid = CollisionManager::GetInstance()->grid;
    collisionGridSize = CollisionManager::GetInstance()->gridSize;
    pathfinding = AIManager::GetInstance()->pathfinding;
    rigidbody = parent->GetComponent<Rigidbody>();
    glm::ivec2 newSpawnPoint = GetRandomPoint();
    parent->transform->SetLocalPosition({newSpawnPoint.x, 0, newSpawnPoint.y});
    subEndPoints.resize(4);
    Component::Start();
}

void CharacterMovement::FixedUpdate() {
#ifdef DEBUG
    ZoneScopedNC("CharacterMovement", 0xfc0f03);
#endif

    currentPosition = parent->transform->GetLocalPosition();

    if (pathIterator >= 0) {
        cellPos = glm::ivec2((int) (currentPosition.x / collisionGridSize) + GRID_SIZE / 2,
                             (int) (currentPosition.z / collisionGridSize) + GRID_SIZE / 2);

        cellPtr = &collisionGrid[cellPos.x + cellPos.y * GRID_SIZE];

        distanceToPoint = glm::distance(currentPosition, (*path)[pathIterator]);

        if (distanceToPoint < DISTANCE_TO_ENDPOINT && subEndPointsIterator < 0 && pathIterator == 0)
            speed = std::lerp(speed, 0.0f, smoothingParam);
        else
            speed = std::lerp(speed, maxSpeed, smoothingParam);

        steeringForce = glm::normalize((*path)[pathIterator] - currentPosition);

        if (!cellPtr->empty()) {
            for (const auto& box: *cellPtr) {
                if (box.first == parent->GetComponent<BoxCollider>()->GetId())
                    continue;

                if (box.second->isDynamic) {
                    steeringPosition = box.second->GetParent()->transform->GetGlobalPosition();
                    distanceToCharacter = glm::distance(currentPosition, steeringPosition);

                    if (distanceToCharacter < maxDistanceToCharacter) {
                        maxDistanceToCharacter = distanceToCharacter;
                        steeringDirection = glm::normalize(currentPosition - steeringPosition);
                    }
                }
            }

            if (maxDistanceToCharacter < DISTANCE_TO_COLLISION) {
                rotationAngle = std::acos(glm::dot(glm::normalize(
                        (*path)[pathIterator] - currentPosition), steeringDirection)) / 2.0f;

                steeringMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

                steeringForce = steeringMatrix * glm::vec4(steeringDirection, 1) * AVOIDANCE_FORCE_MODIFIER;
            }

            maxDistanceToCharacter = FLT_MAX;
        }

        ApplyForces(steeringForce);

        if (distanceToPoint < (DISTANCE_TO_POINT * AIManager::GetInstance()->aiCellSize))
            --pathIterator;
    }

    Component::FixedUpdate();
}

void CharacterMovement::AIUpdate() {
    switch (movementState) {
        case NearTargetPosition:
            SetRandomEndPoint();
            CalculatePath();
            movementState = OnPathToTarget;
            break;
        case NearTargetSubPoint:
            if (subEndPointsIterator >= 0) {
                endPoint = subEndPoints[subEndPointsIterator];
                movementState = OnPathToTarget;
                CalculatePath();
            } else {
                movementState = NearTargetPosition;
            }
            break;
        case OnPathToTarget:
            if (pathIterator < 0) {
                --subEndPointsIterator;
                movementState = NearTargetSubPoint;
            }
            break;
        case SetPathToPlayer:
            SetNewPathToPlayer();
            CalculatePath();
            movementState = OnPathToPlayer;
            break;
        case NearPlayerSubPoint:
            if (subEndPointsIterator >= 0) {
                endPoint = subEndPoints[subEndPointsIterator];
                movementState = OnPathToPlayer;
                CalculatePath();
            } else {
                movementState = NearPlayerPosition;
            }
            break;
        case OnPathToPlayer:
            if (pathIterator < 0) {
                --subEndPointsIterator;
                movementState = NearPlayerSubPoint;
            }
            break;
        case ReturnToPreviousTarget:
            ReturnToPreviousPath();
            CalculatePath();
            movementState = OnPathToTarget;
            break;
        default:
            break;
    }

    Component::AIUpdate();
}

void CharacterMovement::OnDestroy() {
    path->clear();
    delete path;
    rigidbody = nullptr;
    pathfinding = nullptr;
    collisionGrid = nullptr;
    cellPtr = nullptr;
    Component::OnDestroy();
}

inline void CharacterMovement::ApplyForces(const glm::vec3& velocity) {
    rigidbody->AddForce(velocity * speed * speedMultiplier, ForceMode::Force);

    rotationAngle = std::atan2f(-velocity.x, -velocity.z) * 180.0f / std::numbers::pi;

    rigidbody->AddTorque(rotationAngle, ForceMode::Force);
}

void CharacterMovement::SetRandomEndPoint() {
    speed = 0.0f;

    glm::ivec2 newEndPoint = GetRandomPoint();

    endPoint = {newEndPoint.x, 0, newEndPoint.y};

    SetSubEndPoints();
}

const glm::ivec2 CharacterMovement::GetRandomPoint() {
    glm::ivec2 newEndPoint;

    const int aiCellSize = (int)std::ceil(AIManager::GetInstance()->aiCellSize);
    const int aiGridSize = AI_GRID_SIZE / 4 - 1;

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(-aiGridSize / aiCellSize, aiGridSize / aiCellSize);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(-aiGridSize / aiCellSize, aiGridSize / aiCellSize);

        if (!AIManager::GetInstance()->aiGrid[newEndPoint.x + AI_GRID_SIZE / 2][newEndPoint.y + AI_GRID_SIZE / 2])
            break;
    }

    return newEndPoint;
}

void CharacterMovement::SetNewPathToPlayer() {
    previousEndPoint = endPoint;
    speedMultiplier = 2.0f;

    glm::vec3 playerPos = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform->GetLocalPosition();
    int x = (int) playerPos.x, z = (int) playerPos.z;
    bool isAvailable = false;

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
    spdlog::info("p1 " + std::to_string(playerPos.x) + ", " + std::to_string(playerPos.z));
    playerPos = playerPos + GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform->GetForward() * 3.0f;
    spdlog::info("p2 " + std::to_string(playerPos.x) + ", " + std::to_string(playerPos.z));

    SetSubEndPoints();
}

void CharacterMovement::ReturnToPreviousPath() {
    endPoint = previousEndPoint;
    speedMultiplier = 1.0f;
    SetSubEndPoints();
}

void CharacterMovement::SetSubEndPoints() {
#ifdef DEBUG
    ZoneScopedNC("SetSubEndPoints", 0xfc0f09);
#endif

    subEndPoints.clear();
    subEndPointsIterator = 3;

    subEndPoints.push_back(endPoint);

    glm::ivec2 newEndPoint;
    float multiplier = 0.75f;
    int x, y;

    while (multiplier > 0.1f) {
        newEndPoint = {endPoint.x, endPoint.y};
        newEndPoint *= multiplier;

        if (AIManager::GetInstance()->aiGrid[newEndPoint.x + AI_GRID_SIZE / 2][newEndPoint.y + AI_GRID_SIZE / 2]) {
            x = -1, y = -1;

            while (true) {
                newEndPoint = {endPoint.x + x, endPoint.y + y};
                if (!AIManager::GetInstance()->aiGrid[newEndPoint.x + AI_GRID_SIZE / 2][newEndPoint.y + AI_GRID_SIZE / 2])
                    break;

                newEndPoint = {endPoint.x + (x * -1), endPoint.y + y};
                if (!AIManager::GetInstance()->aiGrid[newEndPoint.x + AI_GRID_SIZE / 2][newEndPoint.y + AI_GRID_SIZE / 2])
                    break;

                newEndPoint = {endPoint.x + x, endPoint.y + (y * -1)};
                if (!AIManager::GetInstance()->aiGrid[newEndPoint.x + AI_GRID_SIZE / 2][newEndPoint.y + AI_GRID_SIZE / 2])
                    break;

                newEndPoint = {endPoint.x + (x * -1), endPoint.y + (y * -1)};
                if (!AIManager::GetInstance()->aiGrid[newEndPoint.x + AI_GRID_SIZE / 2][newEndPoint.y + AI_GRID_SIZE / 2])
                    break;

                x *= 2;
                y *= 2;
            }
        }

        subEndPoints.emplace_back(newEndPoint.x, 0.0f, newEndPoint.y);

        multiplier -= 0.25f;
    }
}

void CharacterMovement::SetState(const AI_MOVEMENTSTATE& newState) {
    movementState = newState;
}

const AI_MOVEMENTSTATE CharacterMovement::GetState() const {
    return movementState;
}

void CharacterMovement::CalculatePath() {
#ifdef DEBUG
    ZoneScopedNC("CalculatePath", 0xfc0f03);
#endif

    currentPosition = parent->transform->GetGlobalPosition();

    if (path != nullptr)
        path->clear();
    delete path;

    path = pathfinding->FindNewPath({currentPosition.x, currentPosition.z},
                                    {endPoint.x, endPoint.z});

    if (path == nullptr)
        pathIterator = -1;
    else
        pathIterator = (int)path->size() - 1;
}
