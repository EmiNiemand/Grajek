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

class CharacterMovement : public Component {
    std::shared_ptr<Rigidbody> rigidbody;
    std::vector<glm::vec3> path;
    glm::vec3 endTarget {};
    glm::vec3 previousTarget {};
    glm::vec3 currentPosition {};
    float speed = 0.0f;
    float maxSpeed = 0.5f;
    float smoothingParam = 0.5f;
    bool isMoving = false;
    float rotationAngle = 0.0f;
//    Pathfinding pathFinder;

    float speedMultiplier = 1.0f;
    bool isAlarmed = false;

public:

    CharacterMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterMovement() override;

    void Start() override;
    void FixedUpdate() override;
    void AIUpdate() override;
    void OnDestroy() override;
    void Free();

    void SetNewRandomPoint();
    void SetNewPathToPlayer(glm::vec3 playerPosition);
    void ReturnToPreviousPath();
    void SetNewPathWithPathFinding();
};

#endif //GLOOMENGINE_CHARACTERMOVEMENT_H
