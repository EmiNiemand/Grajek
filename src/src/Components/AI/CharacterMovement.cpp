//
// Created by Adrian on 01.05.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "EngineManagers/CollisionManager.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterMovement::CharacterMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterMovement::~CharacterMovement() = default;

void CharacterMovement::Start() {
    rigidbody = parent->GetComponent<Rigidbody>();
    collisionGrid = CollisionManager::GetInstance()->grid;
    collisionGridSize = CollisionManager::GetInstance()->gridSize;
    otherCharacters = std::make_shared<std::unordered_map<int, std::shared_ptr<CharacterMovement>>>(AIManager::GetInstance()->charactersMovements);
    aiGrid = AIManager::GetInstance()->aiGrid;
    pathfinding = AIManager::GetInstance()->pathfinding;

    glm::vec3 coords;
    coords = AIManager::GetInstance()->minSpawnCoords;
    minSpawnCoords = {coords.x, coords.z};
    coords = AIManager::GetInstance()->maxSpawnCoords;
    maxSpawnCoords = {coords.x, coords.z};

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
            timeSinceLastPoint = 0.0f;
        }
    }

    if (movementState == NearPlayerPosition || movementState == NearDuelPosition) {
        steeringForce = glm::normalize(playerPosition - currentPosition);

        ApplyRotation(steeringForce);
    } else {
        timeSinceLastPoint += GloomEngine::GetInstance()->fixedDeltaTime;

        if (timeSinceLastPoint > MOVEMENT_TIMEOUT) {
            timeSinceLastPoint = 0.0f;
            pathIterator = -1;

            if (movementState == OnPathToPlayer)
                movementState = NearPlayerPosition;
            else if (movementState == OnPathToDuel)
                movementState = NearDuelPosition;
            else
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
        case ReturningToPreviousTarget:
            isStatic = false;
            parent->GetComponent<BoxCollider>()->ChangeAIGridPoints(isStatic);
            otherCharacters->erase(id);
            ReturnToPreviousPath();
            break;
        case OnPathToDuel:
            if (pathIterator < 0)
                movementState = NearDuelPosition;
            break;
        case SettingPathToDuel:
            otherCharacters->insert({id, std::dynamic_pointer_cast<CharacterMovement>(shared_from_this())});
            SetNewPathToPlayer();
            break;
        case NearDuelPosition:
            if (!isStatic) {
                isStatic = true;
                parent->GetComponent<BoxCollider>()->ChangeAIGridPoints(isStatic);
            }
            break;
        case SettingPathToPlayer:
            otherCharacters->insert({id, std::dynamic_pointer_cast<CharacterMovement>(shared_from_this())});
            SetNewPathToPlayer();
            break;
        case OnPathToPlayer:
            if (pathIterator < 0)
                movementState = NearPlayerPosition;
            break;
        case NearPlayerPosition:
            if (!isStatic) {
                isStatic = true;
                parent->GetComponent<BoxCollider>()->ChangeAIGridPoints(isStatic);
            }
            break;
        default:
            break;
    }

    Component::AIUpdate();
}

void CharacterMovement::OnCreate() {
    playerTransform = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform;
    isInitializing = AIManager::GetInstance()->isInitializing;
    Component::OnCreate();
}

void CharacterMovement::OnDestroy() {
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

    if (minX == maxX)
        spdlog::error("SRSPNP X");
    if (minY == maxY)
        spdlog::error("SRSPNP Y");
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
    int minX, maxX, minY, maxY;
    playerPosition = playerTransform->GetLocalPosition();
    glm::ivec2 newEndPoint = {playerPosition.x, playerPosition.z};

    minX = newEndPoint.x - AI_SPAWN_X_MIN_DISTANCE;
    maxX = newEndPoint.x + AI_SPAWN_X_MAX_DISTANCE;
    minY = newEndPoint.y - AI_SPAWN_Y_MIN_DISTANCE;
    maxY = newEndPoint.y + AI_SPAWN_Y_MAX_DISTANCE;

    if (minX == maxX)
        spdlog::error("SRSP X");
    if (minY == maxY)
        spdlog::error("SRSP Y");
    while (true) {
        while (true) {
            newEndPoint.x = RandomnessManager::GetInstance()->GetInt(minSpawnCoords.x, maxSpawnCoords.x);

            if (newEndPoint.x <= minX || newEndPoint.x >= maxX)
                break;
        }

        while (true) {
            newEndPoint.y = RandomnessManager::GetInstance()->GetInt(minSpawnCoords.y, maxSpawnCoords.y);

            if (newEndPoint.y <= minY|| newEndPoint.y >= maxY)
                break;
        }

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

    minX = std::clamp(newEndPoint.x - AI_POINT_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
    maxX = std::clamp(newEndPoint.x + AI_POINT_DISTANCE, minSpawnCoords.x, maxSpawnCoords.x);
    minY = std::clamp(newEndPoint.y - AI_POINT_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);
    maxY = std::clamp(newEndPoint.y + AI_POINT_DISTANCE, minSpawnCoords.y, maxSpawnCoords.y);

    if (minX == maxX)
        spdlog::error("SREP X");
    if (minY == maxY)
        spdlog::error("SREP Y");
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
    int boundariesXY;
    bool isAvailable = false;

    if (movementState == SettingPathToDuel)
        boundariesXY = 4;
    else
        boundariesXY = 3;

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

    distance = glm::distance(glm::vec2(currentPosition.x, currentPosition.z),
                             glm::vec2(newEndPoint.x, newEndPoint.y));

    speedMultiplier = 2.0f + distance / 100.0f;
    previousEndPoint = endPoint;
    endPoint = {newEndPoint.x, newEndPoint.y};

    if (movementState == SettingPathToDuel)
        movementState = OnPathToDuel;
    else
        movementState = OnPathToPlayer;

    CalculatePath(endPoint);
}

/**
 * @annotation
 * Sets previous end point as the current one.
 */
void CharacterMovement::ReturnToPreviousPath() {
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
        if (position == mov.second->GetCurrentEndTarget() && mov.first != id) {
            isAvailable = false;
            break;
        }
    }

    return isAvailable;
}
