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
    otherCharacters = std::make_shared<std::unordered_map<int, std::shared_ptr<CharacterMovement>>>(AIManager::GetInstance()->charactersMovements);
    aiGrid = AIManager::GetInstance()->aiGrid;
    pathfinding = AIManager::GetInstance()->pathfinding;
    minSpawnCoords = AIManager::GetInstance()->minSpawnCoords;
    maxSpawnCoords = AIManager::GetInstance()->maxSpawnCoords;
    rigidbody = parent->GetComponent<Rigidbody>();

    if (isInitializing)
        SetRandomSpawnPointNearPlayer();
    else
        SetRandomSpawnPoint();

    currentPosition = parent->transform->GetLocalPosition();
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

        speed = std::lerp(speed, MOVEMENT_MAX_SPEED, MOVEMENT_SMOOTHING_PARAM);

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
    } else {
        timeSinceLastPlayerPoint += GloomEngine::GetInstance()->fixedDeltaTime;

        if (timeSinceLastPlayerPoint > MOVEMENT_TIMEOUT) {
            timeSinceLastPlayerPoint = 0.0f;
            pathIterator = -1;

            if (movementState == OnPathToPlayer)
                movementState = NearPlayerPosition;
            else if (movementState == OnPathToTarget)
                movementState = NearTargetPosition;
        }
    }

    Component::FixedUpdate();
}

void CharacterMovement::AIUpdate() {
    switch (movementState) {
        case NearTargetPosition:
            SetRandomEndPoint();
            break;
        case OnPathToTarget:
            if (pathIterator < 0)
                movementState = NearTargetPosition;
            break;
        case SettingPathToPlayer:
            SetNewPathToPlayer();
            break;
        case OnPathToPlayer:
            if (pathIterator < 0)
                movementState = NearPlayerPosition;
            break;
        case SettingPathToEnemy:
            SetNewPathToPlayer();
            CalculatePath(endPoint);
            movementState = OnPathToEnemy;
            break;
        case OnPathToEnemy:
            if (pathIterator < 0)
                movementState = NearEnemyPosition;
            break;
        case ReturningToPreviousTarget:
            ReturnToPreviousPath();
            break;
        default:
            break;
    }

    Component::AIUpdate();
}

void CharacterMovement::OnCreate() {
    AIManager::GetInstance()->charactersMovements.insert({id, std::dynamic_pointer_cast<CharacterMovement>(shared_from_this())});
    playerTransform = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform;
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
    int minX, maxX, minY, maxY;
    playerPosition = playerTransform->GetLocalPosition();
    glm::ivec2 newEndPoint = {playerPosition.x, playerPosition.z};

    minX = std::clamp(newEndPoint.x - AI_SPAWN_PLAYER_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
    maxX = std::clamp(newEndPoint.x + AI_SPAWN_PLAYER_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
    minY = std::clamp(newEndPoint.y - AI_SPAWN_PLAYER_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);
    maxY = std::clamp(newEndPoint.y + AI_SPAWN_PLAYER_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(minX, maxX);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(minY, maxY);

        if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
            break;
    }

    parent->transform->SetLocalPosition({newEndPoint.x, 0.01f, newEndPoint.y});
}

/**
 * @annotation
 * Sets random spawn point on AI grid.
 */
void CharacterMovement::SetRandomSpawnPoint() {
    int minX, maxX, minY, maxY, upDown, leftRight;
    playerPosition = playerTransform->GetLocalPosition();
    glm::ivec2 newEndPoint = {playerPosition.x, playerPosition.z};

    upDown = RandomnessManager::GetInstance()->GetInt(0, 1);
    leftRight = RandomnessManager::GetInstance()->GetInt(0, 1);

//    if (upDown) {
//        minX = std::clamp(newEndPoint.x - 2 * AI_SPAWN_GRID_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
//        maxX = std::clamp(newEndPoint.x - AI_SPAWN_GRID_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
//    } else {
//        minX = std::clamp(newEndPoint.x + AI_SPAWN_GRID_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
//        maxX = std::clamp(newEndPoint.x + 2 * AI_SPAWN_GRID_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
//    }
//
//    if (leftRight) {
//        minY = std::clamp(newEndPoint.y - 2 * AI_SPAWN_GRID_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);
//        maxY = std::clamp(newEndPoint.y - AI_SPAWN_GRID_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);
//    } else {
//        minY = std::clamp(newEndPoint.y + AI_SPAWN_GRID_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);
//        maxY = std::clamp(newEndPoint.y + 2 * AI_SPAWN_GRID_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);
//    }

    minX = minSpawnCoords.x;
    maxX = maxSpawnCoords.x;
    minY = minSpawnCoords.y;
    maxY = maxSpawnCoords.y;

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(minX, maxX);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(minY, maxY);

        if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
            break;
    }

    parent->transform->SetLocalPosition({newEndPoint.x, 0.01f, newEndPoint.y});
}

/**
 * @annotation
 * Sets new random point as the end point on AI grid.
 */
void CharacterMovement::SetRandomEndPoint() {
    int minX, maxX, minY, maxY;
    currentPosition = parent->transform->GetLocalPosition();
    glm::ivec2 newEndPoint = {currentPosition.x, currentPosition.z};

    minX = std::clamp(newEndPoint.x - AI_SPAWN_GRID_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
    maxX = std::clamp(newEndPoint.x + AI_SPAWN_GRID_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
    minY = std::clamp(newEndPoint.y - AI_SPAWN_GRID_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);
    maxY = std::clamp(newEndPoint.y + AI_SPAWN_GRID_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);

    if ((rotationAngle < 45.0f && rotationAngle >= 0.0f) || rotationAngle < -135.0f)
        minY = newEndPoint.y;
    else if (rotationAngle >= 45.0f && rotationAngle < 135.0f)
        maxX = newEndPoint.x;
    else if ((rotationAngle >= 135.0f && rotationAngle <= 180.0f) || rotationAngle > -45.0f)
        maxY = newEndPoint.y;
    else
        minX = newEndPoint.x;

    while (true) {
        newEndPoint.x = RandomnessManager::GetInstance()->GetInt(minX, maxX);
        newEndPoint.y = RandomnessManager::GetInstance()->GetInt(minY, maxY);

        if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
            break;
    }

    speed = 0.0f;
    movementState = OnPathToTarget;
    endPoint = {newEndPoint.x, newEndPoint.y};
    CalculatePath(endPoint);
}

/**
 * @annotation
 * Sets new end point near player position.
 */
void CharacterMovement::SetNewPathToPlayer() {
    playerPosition = playerTransform->GetLocalPosition();
    glm::ivec2 newEndPoint, intEndPoint = {playerPosition.x, playerPosition.z};
    int boundariesXY = 2;
    bool isAvailable = false;

    for (int y = -boundariesXY; y <= boundariesXY; y += 2) {
        for (int x = -boundariesXY; x <= boundariesXY; x += 2) {
            if (y == -boundariesXY || y == boundariesXY || x == -boundariesXY || x == boundariesXY) {
                newEndPoint = {intEndPoint.x + x, intEndPoint.y + y};

                if (!aiGrid[(newEndPoint.x + AI_GRID_SIZE / 2) + (newEndPoint.y + AI_GRID_SIZE / 2) * AI_GRID_SIZE])
                    isAvailable = IsPositionAvailable(newEndPoint);

                if (isAvailable)
                    break;
            }
        }

        if (isAvailable)
            break;

        if (y >= boundariesXY) {
            boundariesXY += 2;
            y = -boundariesXY;
        }
    }

    speedMultiplier = 2.0f;
    previousEndPoint = endPoint;
    endPoint = {newEndPoint.x, newEndPoint.y};
    movementState = OnPathToPlayer;

    CalculatePath(endPoint);
}

/**
 * @annotation
 * Sets previous end point as the current one.
 */
void CharacterMovement::ReturnToPreviousPath() {
    endPoint = previousEndPoint;
    speedMultiplier = 1.0f;
    movementState = OnPathToTarget;
    CalculatePath(previousEndPoint);
}

/**
 * @annotation
 * Calculates path using Pathfinding component.
 */
void CharacterMovement::CalculatePath(const glm::ivec2& toPoint) {
#ifdef DEBUG
    ZoneScopedNC("CalculatePath", 0xfc0f03);
#endif

    if (path != nullptr) {
        path->clear();
        delete path;
    }

    path = pathfinding->FindNewPath({currentPosition.x, currentPosition.z},toPoint);

    if (path == nullptr)
        movementState = NearTargetPosition;
    else
        pathIterator = (int)path->size() - 1;
}

/**
 * @annotation
 * Sets new movement state.
 * @param newState - new state to set
 */
void CharacterMovement::SetState(const AI_MOVEMENT_STATE& newState) {
    movementState = newState;
}

/**
 * @annotation
 * Returns current movement state.
 * @returns AI_MOVEMENTSTATE - current state
 */
const AI_MOVEMENT_STATE CharacterMovement::GetState() const {
    return movementState;
}

/**
 * @annotation
 * Returns current end target.
 * @returns glm::ivec2 - endPoint casted to int values
 */
const glm::ivec2 CharacterMovement::GetCurrentEndTarget() const {
    return endPoint;
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
