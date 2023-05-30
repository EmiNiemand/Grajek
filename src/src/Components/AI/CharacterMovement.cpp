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
    otherCharacters = std::make_shared<std::unordered_map<int, std::shared_ptr<CharacterMovement>>>(AIManager::GetInstance()->charactersMovements);
    pathfinding = AIManager::GetInstance()->pathfinding;
    rigidbody = parent->GetComponent<Rigidbody>();

    SetRandomSpawnPoint();
    subEndPoints.resize(4);
    Component::Start();
}

void CharacterMovement::FixedUpdate() {
#ifdef DEBUG
    ZoneScopedNC("CharacterMovement", 0xfc0f03);
#endif

    currentPosition = parent->transform->GetLocalPosition();

    if (movementState == NearPlayerPosition) {
        steeringForce = glm::normalize(GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform->GetLocalPosition() - currentPosition);
        rotationAngle = std::atan2f(-steeringForce.x, -steeringForce.z) * 180.0f / std::numbers::pi;
        rigidbody->AddTorque(rotationAngle, ForceMode::Force);
    }

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

            if (maxDistanceToCharacter < DISTANCE_TO_COLLISION / speedMultiplier) {
                rotationAngle = std::acos(glm::dot(glm::normalize(
                        (*path)[pathIterator] - currentPosition), steeringDirection)) / 2.0f;

                steeringMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

                steeringForce = steeringMatrix * glm::vec4(steeringDirection, 1) * AVOIDANCE_FORCE_MODIFIER;
            }

            maxDistanceToCharacter = FLT_MAX;
        }

        ApplyForces(steeringForce);

        if (subEndPointsIterator < 0 && pathIterator < 1 && movementState == OnPathToPlayer) {
            if (distanceToPoint < DISTANCE_TO_PLAYER)
                --pathIterator;
        } else {
            if (distanceToPoint < DISTANCE_TO_POINT)
                --pathIterator;
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

    rotationAngle = std::atan2f(-velocity.x, -velocity.z) * 180.0f / std::numbers::pi;

    rigidbody->AddTorque(rotationAngle, ForceMode::Force);
}

void CharacterMovement::SetRandomSpawnPoint() {
    glm::ivec2 newSpawnPoint = GetRandomPoint();

    parent->transform->SetLocalPosition({newSpawnPoint.x, 0, newSpawnPoint.y});
}

void CharacterMovement::SetRandomEndPoint() {
    speed = 0.0f;

    glm::ivec2 newEndPoint = GetRandomPoint();

    endPoint = {newEndPoint.x, 0, newEndPoint.y};

    SetSubEndPoints();
}

const glm::ivec2 CharacterMovement::GetRandomPoint() {
    glm::ivec2 newEndPoint;

    // TODO: Change value for the real map size
//    const int aiGridSize = AI_GRID_SIZE / 4 - 1;

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(-24, 24);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(-24, 24);

        if (!AIManager::GetInstance()->aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
            break;
    }

    return newEndPoint;
}

void CharacterMovement::SetNewPathToPlayer() {
    previousEndPoint = endPoint;

    endPoint = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform->GetLocalPosition();

    glm::ivec2 newEndPoint, intEndPoint = {endPoint.x, endPoint.z};

    int minX = -1, maxX = 1, minY = -1, maxY = 1;
    bool isAvailable = false;

    while (true) {
        for (int y = minY; y <= maxY; ++y) {
            if (y > minY && y < maxY) {
                newEndPoint = {intEndPoint.x + minX, intEndPoint.y + y};
                if (!AIManager::GetInstance()->aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
                    isAvailable = IsPositionAvailable(newEndPoint);
                    if (isAvailable)
                        break;
                }

                newEndPoint = {intEndPoint.x + maxX, intEndPoint.y + y};
                if (!AIManager::GetInstance()->aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
                    isAvailable = IsPositionAvailable(newEndPoint);
                    if (isAvailable)
                        break;
                }
            } else {
                for (int x = minX; x <= maxX; ++x) {
                    newEndPoint = {intEndPoint.x + x, intEndPoint.y + y};
                    if (!AIManager::GetInstance()->aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
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

        --minX, ++maxX, --minY, ++maxY;
    }

    speedMultiplier = 2.0f - (float)maxX / 10.0f;
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
    int x, y;

    while (multiplier > 0.1f) {
        newEndPoint = intEndPoint;
        newEndPoint *= multiplier;

        if (AIManager::GetInstance()->aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
            x = -1, y = -1;

            while (true) {
                newEndPoint = {intEndPoint.x + x, intEndPoint.y + y};
                if (!AIManager::GetInstance()->aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    break;

                newEndPoint = {intEndPoint.x + (x * -1), intEndPoint.y + y};
                if (!AIManager::GetInstance()->aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    break;

                newEndPoint = {intEndPoint.x + x, intEndPoint.y + (y * -1)};
                if (!AIManager::GetInstance()->aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    break;

                newEndPoint = {intEndPoint.x + (x * -1), intEndPoint.y + (y * -1)};
                if (!AIManager::GetInstance()->aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    break;

                x -= 1;
                y -= 1;
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

const glm::ivec2 CharacterMovement::GetNewEndTarget() const {
    return {endPoint.x, endPoint.z};
}

const bool CharacterMovement::IsPositionAvailable(const glm::ivec2& position) const {
    bool isAvailable = true;

    for (const auto& mov : *otherCharacters) {
        if (position == mov.second->GetNewEndTarget()) {
            isAvailable = false;
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

    if (path == nullptr)
        pathIterator = -1;
    else
        pathIterator = (int)path->size() - 1;
}
