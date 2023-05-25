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

constexpr float DISTANCE_TO_POINT = 1.0f;
constexpr float DISTANCE_TO_COLLISION = 3.0f;

class GameObject;
class Rigidbody;
class CharacterLogic;
class CharacterPathfinding;

class CharacterMovement : public Component {
    AI_MOVEMENTSTATE movementState = NearTargetSubPoint;
    // Collisions
    float collisionGridSize = 0.0f;
    std::unordered_map<int, std::shared_ptr<BoxCollider>>* collisionGrid = nullptr;
    glm::ivec2 cellPos {};
    std::unordered_map<int, std::shared_ptr<BoxCollider>>* cellPtr = nullptr;
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
    float maxSpeed = 0.05f;
    float speedMultiplier = 1.0f;
    float smoothingParam = 0.5f;
    float rotationAngle = 0.0f;

    void ApplyForces(const glm::vec3 &velocity);
    static const glm::ivec2 GetRandomPoint();
    void SetRandomEndPoint();
    void SetSubEndPoints();
    void CalculatePath();

public:
    CharacterMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterMovement() override;

    void Start() override;
    void FixedUpdate() override;
    void AIUpdate() override;
    void OnDestroy() override;

    void SetNewPathToPlayer();
    void ReturnToPreviousPath();
    const AI_MOVEMENTSTATE GetCurrentStatus() const;

};

#endif //GLOOMENGINE_CHARACTERMOVEMENT_H
