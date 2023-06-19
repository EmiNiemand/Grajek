//
// Created by Adrian on 01.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERMOVEMENT_H
#define GLOOMENGINE_CHARACTERMOVEMENT_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include "Components/AI/CharacterStates.h"
#include <vector>
#include <unordered_map>
#include <numbers>

// Forces consts
constexpr float DISTANCE_TO_POINT = 1.5f;
constexpr float DISTANCE_TO_COLLISION = 1.3f;
constexpr float AVOIDANCE_ROTATION_FACTOR = 1.60f;
constexpr float AVOIDANCE_FORCE_MODIFIER = 1.15f;
// Movement consts
constexpr float MOVEMENT_TIMEOUT = 2.0f;
constexpr float MOVEMENT_SMOOTHING_PARAM = 0.5f;
constexpr float MOVEMENT_MAX_SPEED = 0.075f;

class GameObject;
class Transform;
class BoxCollider;
class Rigidbody;
class CharacterLogic;
class CharacterPathfinding;

class CharacterMovement : public Component {
    AI_MOVEMENT_STATE movementState = NearTargetPosition;
    std::shared_ptr<std::unordered_map<int, std::shared_ptr<CharacterMovement>>> otherCharacters = nullptr;
    float timeSinceLastPoint = 0.0f;
    bool isInitializing = false;
    bool isStatic = false;
    // Collisions
    std::unordered_map<int, std::shared_ptr<BoxCollider>>* collisionGrid = nullptr;
    float collisionGridSize = 0.0f;
    glm::ivec2 cellPos {};
    std::unordered_map<int, std::shared_ptr<BoxCollider>>* cellPtr = nullptr;
    glm::vec3 steeringForce {};
    glm::vec3 steeringPosition {};
    glm::vec3 steeringDirection {};
    glm::mat4 steeringMatrix {};
    float maxDistanceToCharacter = FLT_MAX;
    float distance = 0.0f;
    // Paths and points
    std::shared_ptr<CharacterPathfinding> pathfinding = nullptr;
    const bool *aiGrid = nullptr;
    std::vector<glm::vec3>* path = nullptr;
    int pathIterator = -1;
    std::shared_ptr<Transform> playerTransform = nullptr;
    glm::vec3 playerPosition {};
    glm::vec3 currentPosition {};
    glm::ivec2 endPoint {};
    glm::ivec2 previousEndPoint {};
    glm::ivec2 maxSpawnCoords {};
    glm::ivec2 minSpawnCoords {};
    // Rigidbody parameters
    std::shared_ptr<Rigidbody> rigidbody = nullptr;
    float speed = 0.0f;
    float speedMultiplier = 1.0f;
    float rotationAngle = 0.0f;

    inline void ApplyForces(const glm::vec3 &force);
    inline void ApplyRotation(const glm::vec3 &force);
    void SetRandomSpawnPointNearPlayer();
    void SetRandomSpawnPoint();
    void SetRandomEndPoint();
    void SetNewPathToPlayer();
    void ReturnToPreviousPath();
    void CalculatePath(const glm::ivec2& toPoint);

protected:
    const bool IsPositionAvailable(const glm::ivec2 &position);
    const glm::ivec2 GetCurrentEndTarget() const;

public:
    CharacterMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterMovement() override;

    void Start() override;
    void FixedUpdate() override;
    void AIUpdate() override;
    void OnCreate() override;
    void OnDestroy() override;

    void SetState(const AI_MOVEMENT_STATE& newState);
    const AI_MOVEMENT_STATE GetState() const;

};

#endif //GLOOMENGINE_CHARACTERMOVEMENT_H
