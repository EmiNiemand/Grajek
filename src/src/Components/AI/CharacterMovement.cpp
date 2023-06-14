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

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterMovement::CharacterMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterMovement::~CharacterMovement() = default;

void CharacterMovement::Start() {
    collisionGrid = CollisionManager::GetInstance()->grid;
    collisionGridSize = CollisionManager::GetInstance()->gridSize;
    aiGrid = AIManager::GetInstance()->aiGrid;
    otherCharacters = std::make_shared<std::unordered_map<int, std::shared_ptr<CharacterMovement>>>(AIManager::GetInstance()->charactersMovements);
    pathfinding = AIManager::GetInstance()->pathfinding;
    rigidbody = parent->GetComponent<Rigidbody>();
    playerTransform = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform;
    subEndPoints.resize(AI_GRID_SIZE / 10);
    if (isInitializing)
        SetRandomSpawnPointNearPlayer();
    else
        SetRandomSpawnPoint();
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

        if (pathIterator < 6 && subEndPointsIterator < 0)
            speed = std::lerp(speed, 0.0f, MOVEMENT_SMOOTHING);
        else
            speed = std::lerp(speed, maxSpeed, MOVEMENT_SMOOTHING);

        steeringForce = glm::normalize((*path)[pathIterator] - currentPosition);

        if (!cellPtr->empty()) {
            for (const auto &box: *cellPtr) {
                if (box.first == parent->GetComponent<BoxCollider>()->GetId())
                    continue;

                if (box.second->isDynamic || movementState == OnPathToPlayer) {
                    steeringPosition = box.second->GetParent()->transform->GetGlobalPosition();
                    distance = glm::distance(currentPosition, steeringPosition);

                    if (distance < maxDistanceToCharacter) {
                        maxDistanceToCharacter = distance;
                        steeringDirection = glm::normalize(currentPosition - steeringPosition);
                    }
                }
            }

            if (maxDistanceToCharacter < DISTANCE_TO_COLLISION) {
                rotationAngle = std::acos(glm::dot(steeringForce, steeringDirection)) / AVOIDANCE_ROTATION_FACTOR;

                if (rotationAngle > 0.4f) {
                    steeringMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
                    steeringForce = steeringMatrix * glm::vec4(steeringDirection, 1) * AVOIDANCE_FORCE_MODIFIER;
                }
            }

            maxDistanceToCharacter = FLT_MAX;
        }

        ApplyForces(steeringForce);

        distance = glm::distance(currentPosition, (*path)[pathIterator]);

        if (distance < DISTANCE_TO_POINT) {
            --pathIterator;
            timeSinceLastPlayerPoint = 0.0f;
        }
    }


    if (movementState == NearPlayerPosition) {
        steeringForce = glm::normalize(playerPosition - currentPosition);

        ApplyRotation(steeringForce);
    } else if (movementState == OnPathToPlayer) {
        timeSinceLastPlayerPoint += GloomEngine::GetInstance()->fixedDeltaTime;

        if (timeSinceLastPlayerPoint > MOVEMENT_TIMEOUT) {
            timeSinceLastPlayerPoint = 0.0f;
            subEndPointsIterator = -1;
            pathIterator = -1;
            movementState = NearPlayerSubPoint;
        }
    }

    if (subEndPointsIterator < 0) {
        if (movementState == NearPlayerSubPoint)
            movementState = NearPlayerPosition;
        else if (movementState == NearTargetSubPoint)
            movementState = NearTargetPosition;
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
        case SettingPathToPlayer:
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
        case SettingPathToEnemy:
            SetNewPathToPlayer();
            CalculatePath();
            movementState = OnPathToEnemy;
            break;
        case NearEnemySubPoint:
            if (subEndPointsIterator >= 0) {
                endPoint = subEndPoints[subEndPointsIterator];
                movementState = OnPathToEnemy;
                CalculatePath();
            }
            break;
        case OnPathToEnemy:
            if (pathIterator < 0) {
                --subEndPointsIterator;
                movementState = NearEnemySubPoint;
            }
            break;
        case ReturningToPreviousTarget:
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
    isInitializing = AIManager::GetInstance()->isInitializing;
    Component::OnCreate();
}

void CharacterMovement::OnDestroy() {
    AIManager::GetInstance()->RemoveCharacterMovement(id);
    if (path != nullptr) {
        path->clear();
        delete path;
    }
    rigidbody = nullptr;
    pathfinding = nullptr;
    collisionGrid = nullptr;
    playerTransform = nullptr;
    Component::OnDestroy();
}

/**
 * @annotation
 * Applies all forces based on force value.
 * @param velocity - force
 */
inline void CharacterMovement::ApplyForces(const glm::vec3& force) {
    rigidbody->AddForce(force * speed * speedMultiplier, ForceMode::Force);

    ApplyRotation(force);
}

/**
 * @annotation
 * Applies rotation based on force value.
 * @param velocity - force
 */
inline void CharacterMovement::ApplyRotation(const glm::vec3& force) {
    rotationAngle = std::atan2f(-force.x, -force.z) * 180.0f / std::numbers::pi_v<float>;

    rigidbody->AddTorque(rotationAngle, ForceMode::Force);
}

/**
 * @annotation
 * Sets random spawn point on AI grid near player position.
 */
void CharacterMovement::SetRandomSpawnPointNearPlayer() {
    playerPosition = playerTransform->GetLocalPosition();
    int minX, maxX, minY, maxY;
    glm::ivec2 newEndPoint = {playerPosition.x, playerPosition.z};

    minX = std::clamp(newEndPoint.x - AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    maxX = std::clamp(newEndPoint.x + AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    minY = std::clamp(newEndPoint.y - AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    maxY = std::clamp(newEndPoint.y + AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(minX, maxX);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(minY, maxY);

        if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
            break;
    }

    endPoint = {newEndPoint.x, 0.01f, newEndPoint.y};

    parent->transform->SetLocalPosition(endPoint);
}

/**
 * @annotation
 * Sets random spawn point on AI grid.
 */
void CharacterMovement::SetRandomSpawnPoint() {
    playerPosition = playerTransform->GetLocalPosition();
    int minX, maxX, minY, maxY, upDown, leftRight;
    glm::ivec2 newEndPoint = {playerPosition.x, playerPosition.z};

    upDown = RandomnessManager::GetInstance()->GetInt(0, 1);
    leftRight = RandomnessManager::GetInstance()->GetInt(0, 1);

    if (upDown) {
        minX = std::clamp(newEndPoint.x - AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
        maxX = std::clamp(newEndPoint.x, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    } else {
        minX = std::clamp(newEndPoint.x, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
        maxX = std::clamp(newEndPoint.x + AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    }

    if (leftRight) {
        minY = std::clamp(newEndPoint.y - AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
        maxY = std::clamp(newEndPoint.y, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    } else {
        minY = std::clamp(newEndPoint.y, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
        maxY = std::clamp(newEndPoint.y + AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    }

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(minX, maxX);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(minY, maxY);

        if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
            break;
    }

    endPoint = {newEndPoint.x, 0.01f, newEndPoint.y};

    parent->transform->SetLocalPosition(endPoint);
}

/**
 * @annotation
 * Sets new random point as the end point on AI grid.
 */
void CharacterMovement::SetRandomEndPoint() {
    speed = 0.0f;

    endPoint = GetRandomPoint();

    SetSubEndPoints();
}

/**
 * @annotation
 * Returns new random point on AI grid.
 * @returns glm::ivec2 - random point in a given grid
 */
const glm::vec3 CharacterMovement::GetRandomPoint() {
    currentPosition = parent->transform->GetLocalPosition();
    glm::ivec2 newEndPoint = {currentPosition.x, currentPosition.z};
    int minX, maxX, minY, maxY;

    minX = std::clamp(newEndPoint.x - AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    maxX = std::clamp(newEndPoint.x + AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    minY = std::clamp(newEndPoint.y - AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);
    maxY = std::clamp(newEndPoint.y + AI_SPAWN_GRID_DISTANCE, -AI_GRID_SIZE / 2 + 1, AI_GRID_SIZE / 2 - 1);

    if (rotationAngle > 225.0f && rotationAngle < 315.0f)
        minX = newEndPoint.x;
    else if (rotationAngle > 135.0f && rotationAngle < 225.0f)
        maxY = newEndPoint.y;
    else if (rotationAngle > 45.0f && rotationAngle < 135.0f)
         maxX = newEndPoint.x;
    else
        minY = newEndPoint.y;

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(minX, maxX);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(minY, maxY);

        if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
            break;
    }

    return {newEndPoint.x, 0.01f, newEndPoint.y};
}

/**
 * @annotation
 * Sets new end point near player position.
 */
void CharacterMovement::SetNewPathToPlayer() {
    previousEndPoint = endPoint;

    playerPosition = playerTransform->GetLocalPosition();

    glm::ivec2 newEndPoint, intEndPoint = {playerPosition.x, playerPosition.z};

    int minX = -2, maxX = 2, minY = -2, maxY = 2;
    bool isAvailable = false;

    for (int y = minY; y <= maxY; y += 2) {
        for (int x = minX; x <= maxX; x += 2) {
            if (y == minY || y == maxY || x == minX || x == maxX) {
                newEndPoint = {intEndPoint.x + x, intEndPoint.y + y};

                if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    isAvailable = IsPositionAvailable(newEndPoint);

                if (isAvailable)
                    break;
            }
        }

        if (isAvailable)
            break;

        if (y >= maxY) {
            minX -= 2, maxX += 2, minY -= 2, maxY += 2;
            y = minY;
        }
    }

    speedMultiplier = 2.0f;
    endPoint = {newEndPoint.x, 0, newEndPoint.y};

    SetSubEndPoints();
}

/**
 * @annotation
 * Sets previous end point as the current one.
 */
void CharacterMovement::ReturnToPreviousPath() {
    endPoint = previousEndPoint;
    speedMultiplier = 1.0f;
    SetSubEndPoints();
}

/**
 * @annotation
 * Sets sub end points for new end target.
 */
void CharacterMovement::SetSubEndPoints() {
#ifdef DEBUG
    ZoneScopedNC("SetSubEndPoints", 0xfc0f09);
#endif

    int subPointsAmount = (int)std::floor(glm::distance(currentPosition, endPoint) / 10.0f);

    subEndPointsIterator = subPointsAmount;

    subEndPoints.insert(subEndPoints.cbegin() + subEndPointsIterator, endPoint);

    if (subPointsAmount == 0)
        return;

    glm::ivec2 newEndPoint = {}, mulEndPoint = {};
    int X = 1, Y = 1;
    float multiplier = 1.0f / ((float)subPointsAmount + 1.0f);
    float multiplierCounter = (float)subPointsAmount - multiplier;

    while (multiplierCounter > 0.1f && subEndPointsIterator > 1) {
        --subEndPointsIterator;
        mulEndPoint = {endPoint.x, endPoint.z};
        mulEndPoint *= multiplier;

        if (aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
            while (true) {
                newEndPoint = {mulEndPoint.x + X, mulEndPoint.y + Y};
                if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    break;

                newEndPoint = {mulEndPoint.x - X, mulEndPoint.y + Y};
                if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    break;

                newEndPoint = {mulEndPoint.x + X, mulEndPoint.y - Y};
                if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    break;

                newEndPoint = {mulEndPoint.x - X, mulEndPoint.y - Y};
                if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    break;

                ++X, ++Y;
            }
        }

        subEndPoints.insert(subEndPoints.cbegin() + subEndPointsIterator, {newEndPoint.x, 0.0f, newEndPoint.y});
        multiplierCounter -= multiplier;
    }

    subEndPointsIterator = subPointsAmount;
}

/**
 * @annotation
 * Calculates path using Pathfinding component.
 */
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

/**
 * @annotation
 * Sets new movement state.
 * @param newState - new state to set
 */
void CharacterMovement::SetState(const AI_MOVEMENTSTATE& newState) {
    movementState = newState;
}

/**
 * @annotation
 * Returns current movement state.
 * @returns AI_MOVEMENTSTATE - current state
 */
const AI_MOVEMENTSTATE CharacterMovement::GetState() const {
    return movementState;
}

/**
 * @annotation
 * Returns current end target.
 * @returns glm::ivec2 - endPoint casted to int values
 */
const glm::ivec2 CharacterMovement::GetCurrentEndTarget() const {
    return {endPoint.x, endPoint.z};
}

/**
 * @annotation
 * Checks whether this position is already chosen by another CharacterMovement object
 * @param position - position to check
 * @returns bool - false if not available, otherwise true
 */
const bool CharacterMovement::IsPositionAvailable(const glm::ivec2& position) {
    bool isAvailable = true;

    for (const auto& mov : *otherCharacters) {
        if (position == mov.second->GetCurrentEndTarget()) {
            isAvailable = false;
            break;
        }
    }

    return isAvailable;
}
