//
// Created by Adrian on 01.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERMOVEMENT_H
#define GLOOMENGINE_CHARACTERMOVEMENT_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "EngineManagers/CollisionManager.h"
#include "Components/Component.h"
#include <vector>
#include <unordered_map>

class GameObject;
class Rigidbody;

class CharacterMovement : public Component {
    AI_LOGICSTATE logicState = WalkingOnPath;
    std::shared_ptr<Rigidbody> rigidbody;
    std::vector<glm::vec3>* path;
    glm::vec3 newPosition {};
    glm::vec3 endTarget {};
    glm::vec3 previousTarget {};
    glm::vec3 currentPosition {};
    float speed = 0.0f;
    float maxSpeed = 0.5f;
    float speedMultiplier = 1.0f;
    float smoothingParam = 0.5f;
    float rotationAngle = 0.0f;

public:
    CharacterMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterMovement() override;

    void FixedUpdate() override;
    void AIUpdate() override;
    void OnCreate() override;
    void OnDestroy() override;
    void Free();

    void SetNewRandomPoint();
    void SetNewPath(AI_LOGICSTATE state);
    void CalculateNewPath();

};

#endif //GLOOMENGINE_CHARACTERMOVEMENT_H
