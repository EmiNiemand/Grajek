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

constexpr float DISTANCE_TO_POINT = 1.8f;
constexpr float DISTANCE_TO_COLLISION = 2.0f;
constexpr float AVOIDANCE_ROTATION_FACTOR = 1.8f;
constexpr float AVOIDANCE_FORCE_MODIFIER = 1.3f;
constexpr float MOVEMENT_TIMEOUT = 3.0f;

class GameObject;
class Rigidbody;
class CharacterLogic;
class CharacterPathfinding;

class CharacterMovement : public Component {
    AI_MOVEMENTSTATE movementState = NearTargetPosition;
    std::shared_ptr<std::unordered_map<int, std::shared_ptr<CharacterMovement>>> otherCharacters = nullptr;
    float timeSinceLastPoint = 0.0f;
    bool isInitializing = false;
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
    glm::vec3 playerPosition {};
    glm::vec3 currentPosition {};
    glm::vec3 endPoint {};
    glm::vec3 previousEndPoint {};
    std::vector<glm::vec3> subEndPoints {};
    int subEndPointsIterator = -1;
    // Rigidbody parameters
    std::shared_ptr<Rigidbody> rigidbody = nullptr;
    float speed = 0.0f;
    float maxSpeed = 0.08f;
    float speedMultiplier = 1.0f;
    float smoothingParam = 0.5f;
    float rotationAngle = 0.0f;

    inline void ApplyForces(const glm::vec3 &force);
    inline void ApplyRotation(const glm::vec3 &force);
    void SetRandomSpawnPointNearPlayer();
    void SetRandomSpawnPoint();
    const glm::vec3 GetRandomPoint();
    void SetRandomEndPoint();
    void SetNewPathToPlayer();
    void ReturnToPreviousPath();
    void SetSubEndPoints();
    void CalculatePath();

protected:
    const bool IsPositionAvailable(const glm::ivec2 &position);
    const glm::ivec2 GetCurrentEndTarget() const;

public:
    CharacterMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterMovement() override;

    void Awake() override;
    void FixedUpdate() override;
    void AIUpdate() override;
    void OnCreate() override;
    void OnDestroy() override;

    void SetState(const AI_MOVEMENTSTATE& newState);
    const AI_MOVEMENTSTATE GetState() const;

};

#endif //GLOOMENGINE_CHARACTERMOVEMENT_H
