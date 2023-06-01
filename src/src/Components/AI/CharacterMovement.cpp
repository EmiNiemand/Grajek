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

void CharacterMovement::Awake() {
    collisionGrid = CollisionManager::GetInstance()->grid;
    collisionGridSize = CollisionManager::GetInstance()->gridSize;
    aiGrid = AIManager::GetInstance()->aiGrid;
    aiCellSize = AIManager::GetInstance()->aiCellSize;
    otherCharacters = std::make_shared<std::unordered_map<int, std::shared_ptr<CharacterMovement>>>(AIManager::GetInstance()->charactersMovements);
    pathfinding = AIManager::GetInstance()->pathfinding;
    rigidbody = parent->GetComponent<Rigidbody>();
    SetRandomSpawnPoint();
    subEndPoints.resize(4);
    Component::Awake();
}

void CharacterMovement::FixedUpdate() {
#ifdef DEBUG
    ZoneScopedNC("CharacterMovement", 0xfc0f03);
#endif

    currentPosition = parent->transform->GetLocalPosition();

    if (movementState == NearPlayerPosition) {
        steeringForce = glm::normalize(playerPosition - currentPosition);

        ApplyRotation(steeringForce);
    }

    if (pathIterator >= 0) {
        cellPos = glm::ivec2((int) (currentPosition.x / collisionGridSize) + GRID_SIZE / 2,
                             (int) (currentPosition.z / collisionGridSize) + GRID_SIZE / 2);

        cellPtr = &collisionGrid[cellPos.x + cellPos.y * GRID_SIZE];

        if (subEndPointsIterator < 0 && pathIterator < 4)
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
                    distance = glm::distance(currentPosition, steeringPosition);

                    if (distance < maxDistanceToCharacter) {
                        maxDistanceToCharacter = distance;
                        steeringDirection = glm::normalize(currentPosition - steeringPosition);
                    }
                }
            }

            if (maxDistanceToCharacter < DISTANCE_TO_COLLISION / speedMultiplier) {
                rotationAngle = std::acos(glm::dot(glm::normalize(
                        (*path)[pathIterator] - currentPosition), steeringDirection)) / 2.0f;

                steeringMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

                steeringForce = steeringMatrix * glm::vec4(steeringDirection, 1) * AVOIDANCE_FORCE_MODIFIER;
            }

            maxDistanceToCharacter = FLT_MAX;
        }

        ApplyForces(steeringForce);

        distance = glm::distance(currentPosition, (*path)[pathIterator]);

        if (subEndPointsIterator < 0 && pathIterator == 2 && movementState == OnPathToPlayer) {
            if (distance < DISTANCE_TO_PLAYER)
                --pathIterator;
        } else {
            if (distance < DISTANCE_TO_POINT)
                --pathIterator;
        }

        if (subEndPointsIterator < 0 && pathIterator < 0) {
            if (movementState == OnPathToPlayer)
                movementState = NearPlayerPosition;
            else if (movementState == OnPathToTarget)
                movementState = NearTargetPosition;
            else if (movementState == OnPathToEnemy)
                movementState = NearEnemyPosition;
        }
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

void CharacterMovement::OnCreate() {
    AIManager::GetInstance()->charactersMovements.insert({id, std::dynamic_pointer_cast<CharacterMovement>(shared_from_this())});
    Component::OnCreate();
}

void CharacterMovement::OnDestroy() {
    AIManager::GetInstance()->RemoveCharacterMovement(id);
    path->clear();
    delete path;
    rigidbody = nullptr;
    pathfinding = nullptr;
    collisionGrid = nullptr;
    Component::OnDestroy();
}

inline void CharacterMovement::ApplyForces(const glm::vec3& velocity) {
    rigidbody->AddForce(velocity * speed * speedMultiplier, ForceMode::Force);

    ApplyRotation(velocity);
}

inline void CharacterMovement::ApplyRotation(const glm::vec3& velocity) {
    rotationAngle = std::atan2f(-velocity.x, -velocity.z) * 180.0f / std::numbers::pi;

    rigidbody->AddTorque(rotationAngle, ForceMode::Force);
}

inline void CharacterMovement::SetRandomSpawnPoint() {
    glm::ivec2 newSpawnPoint = GetRandomPoint();

    parent->transform->SetLocalPosition({newSpawnPoint.x, 0.01f, newSpawnPoint.y});
}

void CharacterMovement::SetRandomEndPoint() {
    speed = 0.0f;

    glm::ivec2 newEndPoint = GetRandomPoint();

    endPoint = {newEndPoint.x, 0, newEndPoint.y};

    SetSubEndPoints();
}

const glm::ivec2 CharacterMovement::GetRandomPoint() const {
    glm::ivec2 newEndPoint;

    // TODO: Change value for the real map size
    //  const int aiGridSize = AI_GRID_SIZE / 4 - 1;

    const int aiGridSize = (int)std::round(24.0f / aiCellSize);

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(-aiGridSize, aiGridSize);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(-aiGridSize, aiGridSize);

        if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
            break;
    }

    return newEndPoint;
}

void CharacterMovement::SetNewPathToPlayer() {
    previousEndPoint = endPoint;

    playerPosition = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform->GetLocalPosition();

    glm::ivec2 newEndPoint, intEndPoint = {playerPosition.x, playerPosition.z};

    int minX = -2, maxX = 2, minY = -2, maxY = 2;
    bool isAvailable = false;

    while (true) {
        for (int y = minY; y <= maxY; y += 2) {
            if (y > minY && y < maxY) {
                newEndPoint = {intEndPoint.x + minX, intEndPoint.y + y};
                if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
                    isAvailable = IsPositionAvailable(newEndPoint);
                    if (isAvailable)
                        break;
                }

                newEndPoint = {intEndPoint.x + maxX, intEndPoint.y + y};
                if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
                    isAvailable = IsPositionAvailable(newEndPoint);
                    if (isAvailable)
                        break;
                }
            } else {
                for (int x = minX; x <= maxX; x += 2) {
                    newEndPoint = {intEndPoint.x + x, intEndPoint.y + y};
                    if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
                        isAvailable = IsPositionAvailable(newEndPoint);
                        if (isAvailable)
                            break;
                    }
                }

                if (isAvailable)
                    break;
            }
        }

        if (isAvailable)
            break;

        minX -= 2, maxX += 2, minY -= 2, maxY += 2;
    }

    speedMultiplier = 2.0f - (float) maxX / 10.0f;
    endPoint = {newEndPoint.x, 0, newEndPoint.y};

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

    glm::ivec2 newEndPoint, intEndPoint = {endPoint.x, endPoint.z};
    float multiplier = 0.75f;
    int minX = -1, maxX = 1, minY = -1, maxY = 1;
    bool isAvailable = false;

    while (multiplier > 0.1f) {
        newEndPoint = intEndPoint;
        newEndPoint *= multiplier;

        if (aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
            for (int y = minY; y <= maxY; ++y) {
                for (int x = minX; x <= maxX; ++x) {
                    newEndPoint = {intEndPoint.x + x, intEndPoint.y + y};

                    if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
                        isAvailable = true;
                        break;
                    }

                    if (y > minY && y < maxY)
                        x = maxX - 1;
                }

                if (isAvailable)
                    break;

                if (y == maxY)
                    --minX, ++maxX, --minY, ++maxY;
            }
        }

        subEndPoints.emplace_back(newEndPoint.x, 0.0f, newEndPoint.y);

        isAvailable = false;
        multiplier -= 0.25f;
    }
}

void CharacterMovement::SetState(const AI_MOVEMENTSTATE& newState) {
    movementState = newState;
}

const AI_MOVEMENTSTATE CharacterMovement::GetState() const {
    return movementState;
}

const glm::ivec2 CharacterMovement::GetNewEndTarget() const {
    return {endPoint.x, endPoint.z};
}

const bool CharacterMovement::IsPositionAvailable(const glm::ivec2& position) {
    bool isAvailable = true;

    for (const auto& mov : *otherCharacters) {
        if (position == mov.second->GetNewEndTarget()) {
            isAvailable = false;
            break;
        }
    }

    return isAvailable;
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

//    spdlog::info("path");
//    for (const auto&n : *path)
//        spdlog::info(std::to_string(n.x) + ", " + std::to_string(n.z));

    if (path == nullptr)
        pathIterator = -1;
    else
        pathIterator = (int)path->size() - 1;
}
