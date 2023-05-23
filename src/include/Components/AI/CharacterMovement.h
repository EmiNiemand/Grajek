//
// Created by Adrian on 01.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERMOVEMENT_H
#define GLOOMENGINE_CHARACTERMOVEMENT_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "EngineManagers/AIManager.h"
#include "Components/Component.h"
#include <vector>
#include <unordered_map>

class GameObject;
class Rigidbody;

class CharacterMovement : public Component {
    AI_LOGICSTATE logicState = WalkingOnPath;
    std::shared_ptr<Rigidbody> rigidbody = nullptr;
    std::shared_ptr<CharacterPathfinding> pathfinding = nullptr;
    std::vector<glm::vec3> path;
    int pathIterator = -1;
    glm::vec3 newPosition {};
    glm::vec3 endTarget {};
    glm::vec3 previousTarget {};
    glm::vec3 currentPosition {};
    float speed = 0.0f;
    float maxSpeed = 0.5f;
    float speedMultiplier = 1.0f;
    float smoothingParam = 0.5f;
    float rotationAngle = 0.0f;

    static const glm::vec3 GetNewSpawnPoint();
    void SetNewRandomEndPoint();
    void CalculateNewPath();

public:
    CharacterMovement(const std::shared_ptr<GameObject> &parent, int id);
    ~CharacterMovement() override;

    void FixedUpdate() override;
    void AIUpdate() override;
    void OnCreate() override;
    void OnDestroy() override;
    void Free();

    void SetNewPath(AI_LOGICSTATE state);

};

#endif //GLOOMENGINE_CHARACTERMOVEMENT_H
