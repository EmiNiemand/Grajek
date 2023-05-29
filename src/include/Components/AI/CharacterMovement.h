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

constexpr float DISTANCE_TO_ENDPOINT = 1.0f;
constexpr float DISTANCE_TO_POINT = 1.75f;
constexpr float DISTANCE_TO_COLLISION = 2.3f;
constexpr float AVOIDANCE_FORCE_MODIFIER = 1.1f;

class GameObject;
class Rigidbody;
class CharacterLogic;
class CharacterPathfinding;

class CharacterMovement : public Component {
    AI_MOVEMENTSTATE movementState = NearTargetPosition;
    // Collisions
    float collisionGridSize = 0.0f;
    std::unordered_map<int, std::shared_ptr<BoxCollider>>* collisionGrid = nullptr;
    glm::ivec2 cellPos {};
    std::unordered_map<int, std::shared_ptr<BoxCollider>>* cellPtr = nullptr;
    glm::vec3 steeringForce {};
    glm::vec3 steeringPosition {};
    glm::vec3 steeringDirection {};
    glm::mat4 steeringMatrix {};
    float maxDistanceToCharacter = FLT_MAX;
    float distanceToCharacter = 0.0f;
    float distanceToPoint = 0.0f;
    // Paths and points
    std::shared_ptr<CharacterPathfinding> pathfinding = nullptr;
    std::vector<glm::vec3>* path = nullptr;
    int pathIterator = -1;
    glm::vec3 currentPosition {};
    glm::vec3 endPoint {};
    glm::vec3 previousEndPoint {};
    std::vector<glm::vec3> subEndPoints {};
    int subEndPointsIterator = -1;
    // Parameters for Rigidbody
    std::shared_ptr<Rigidbody> rigidbody = nullptr;
    float speed = 0.0f;
    float maxSpeed = 0.08f;
    float speedMultiplier = 1.0f;
    float smoothingParam = 0.5f;
    float rotationAngle = 0.0f;

    inline void ApplyForces(const glm::vec3 &velocity);
    static const glm::ivec2 GetRandomPoint();
    void SetRandomEndPoint();
    void SetSubEndPoints();
    void CalculatePath();
    void SetNewPathToPlayer();
    void ReturnToPreviousPath();

public:
    CharacterMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterMovement() override;

    void Start() override;
    void FixedUpdate() override;
    void AIUpdate() override;
    void OnDestroy() override;

    void SetState(const AI_MOVEMENTSTATE& newState);
    const AI_MOVEMENTSTATE GetState() const;

};

#endif //GLOOMENGINE_CHARACTERMOVEMENT_H
