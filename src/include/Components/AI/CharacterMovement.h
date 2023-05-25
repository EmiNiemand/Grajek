//
// Created by Adrian on 01.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERMOVEMENT_H
#define GLOOMENGINE_CHARACTERMOVEMENT_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include <vector>
#include <unordered_map>

class GameObject;
class Rigidbody;
class CharacterPathfinding;

class CharacterMovement : public Component {
    friend class CharacterLogic;
    AI_LOGICSTATE logicState = WalkingOnPath;
    bool alreadyCalled = false;
    // Collisions
    float collisionGridSize = 0.0f;
    std::unordered_map<int, std::shared_ptr<BoxCollider>>* collisionGrid = nullptr;
    // Paths and points
    std::shared_ptr<CharacterPathfinding> pathfinding = nullptr;
    std::vector<glm::vec3>* path = nullptr;
    int pathIterator = -1;
    glm::vec3 currentPosition {};
    glm::vec3 newPosition {};
    glm::vec3 endPoint {};
    glm::vec3 previousEndPoint {};
    std::vector<glm::vec3> subEndPoints {};
    int endPointsIterator = -1;
    // Parameters for Rigidbody
    std::shared_ptr<Rigidbody> rigidbody = nullptr;
    float speed = 0.0f;
    float maxSpeed = 0.1f;
    float speedMultiplier = 1.0f;
    float smoothingParam = 0.5f;
    float rotationAngle = 0.0f;

    static const glm::vec3 GetNewSpawnPoint();
    void SetRandomEndPoint();
    void SetSubEndPoints();
    void SetNewPathToPlayer();
    void ReturnToPreviousPath();
    void CalculatePath();

public:
    CharacterMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterMovement() override;

    void Start() override;
    void FixedUpdate() override;
    void AIUpdate() override;
    void OnDestroy() override;

};

#endif //GLOOMENGINE_CHARACTERMOVEMENT_H
