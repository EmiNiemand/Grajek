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
private:
    friend class CharacterLogic;
    AI_LOGICSTATE logicState = WalkingOnPath;
    std::shared_ptr<Rigidbody> rigidbody = nullptr;
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
    float speed = 0.0f;
    float maxSpeed = 0.3f;
    float speedMultiplier = 1.0f;
    float smoothingParam = 0.5f;
    float rotationAngle = 0.0f;

    static const glm::vec3 GetNewSpawnPoint();
    void SetRandomEndPoint();
    void SetSubEndPoints();
    void CalculatePath();

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
