//
// Created by Adrian on 01.05.2023.
//

#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/AI/CharacterStates.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include <numbers>

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterMovement::CharacterMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterMovement::~CharacterMovement() = default;

void CharacterMovement::Start() {
    playerTransform = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform;
    rigidbody = parent->GetComponent<Rigidbody>();
    SetNewRandomPoint();
    parent->transform->SetLocalPosition(endTarget);
    SetNewRandomPoint();
    Component::Start();
}

void CharacterMovement::FixedUpdate() {
    currentPosition = parent->transform->GetLocalPosition();

    if (!path.empty()) {
        speed = std::lerp(speed, maxSpeed, smoothingParam);

        newPosition = glm::normalize(path[0] - currentPosition) * speed * speedMultiplier;

        rigidbody->AddForce(newPosition, ForceMode::Force);

        rotationAngle = std::atan2f(-newPosition.x, -newPosition.z) * 180.0f/std::numbers::pi;

        if (rotationAngle < 0.0f) {
            rotationAngle += 360.0f;
        }

        rigidbody->AddTorque(rotationAngle, ForceMode::Force);
    }

    Component::FixedUpdate();
}

void CharacterMovement::AIUpdate() {
    if (!path.empty()) {
        if (glm::distance(currentPosition, path[0]) < 1.0f)
            path.erase(path.begin());
    }

    if (path.empty() && logicState != WalkingToPlayer) {
        SetNewRandomPoint();
        CalculateNewPath();
    }

    Component::AIUpdate();
}

void CharacterMovement::OnDestroy() {
    rigidbody = nullptr;
    path.clear();
    Component::OnDestroy();
}

void CharacterMovement::Free() {
    rigidbody = nullptr;
    path.clear();
}

void CharacterMovement::SetNewRandomPoint() {
    endTarget.x = RandomnessManager::GetInstance()->GetFloat(-25, 25);
    endTarget.z = RandomnessManager::GetInstance()->GetFloat(-25, 25);
}

void CharacterMovement::SetNewPath(AI_STATE state) {
    logicState = state;

    if (logicState == WalkingToPlayer) {
        previousTarget = endTarget;
        endTarget = playerTransform->GetLocalPosition();
        endTarget.x -= RandomnessManager::GetInstance()->GetFloat(0.5f, 2.0f);
        endTarget.z -= RandomnessManager::GetInstance()->GetFloat(0.5f, 2.0f);
        speedMultiplier = 2.0f;
    } else if (logicState == TraversingOnPath) {
        endTarget = previousTarget;
        speedMultiplier = 1.0f;
    }

    CalculateNewPath();
}

void CharacterMovement::CalculateNewPath() {
    speed = 0.0f;
    path.clear();
    path.push_back(endTarget);
//    foreach (var obj in GameObject.FindGameObjectsWithTag("Obstacles"))
//    {
//        var col = obj.GetComponent<BoxCollider>();
//        if (col && ((transform.position.y + bCollider.height / 2 > obj.transform.position.y - col.size.y / 2 &&
//                     transform.position.y - bCollider.height / 2 < obj.transform.position.y - col.size.y / 2) ||
//                    (transform.position.y - bCollider.height / 2 < obj.transform.position.y + col.size.y / 2 &&
//                     transform.position.y + bCollider.height / 2 > obj.transform.position.y + col.size.y / 2) ||
//                    (transform.position.y + bCollider.height / 2 < obj.transform.position.y + col.size.y / 2 &&
//                     transform.position.y - bCollider.height / 2 > obj.transform.position.y - col.size.y / 2)))
//        {
//            if (!hitColliders.ContainsKey(col.GetInstanceID())) hitColliders.Add(col.GetInstanceID(), col);
//        }
//    }

//    path = pathFinder.FindPath(currentPosition, endTarget);
}

