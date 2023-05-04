//
// Created by Adrian on 01.05.2023.
//

#include <numbers>
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterMovement::CharacterMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterMovement::~CharacterMovement() = default;

void CharacterMovement::Start() {
    rigidbody = parent->GetComponent<Rigidbody>();
    SetNewRandomPoint();
    SetNewPathWithPathFinding();
    Component::Start();
}

void CharacterMovement::Update() {
    currentPosition = parent->transform->GetLocalPosition();

    if (!path.empty()) {
        speed = std::lerp(speed, maxSpeed, smoothingParam);

        glm::vec3 newPos = {path.begin()->x - currentPosition.x, 0, path.begin()->z - currentPosition.z};
        newPos = glm::normalize(newPos);
        newPos *= (speed * speedMultiplier);

        rigidbody->AddForce(newPos, ForceMode::Force);

        rotationAngle = std::atan2f(-newPos.x, -newPos.z) * 180.0f/std::numbers::pi;

        if (rotationAngle < 0.0f) {
            rotationAngle += 360.0f;
        }

        rigidbody->AddTorque(rotationAngle, ForceMode::Force);

        if (glm::distance(currentPosition, path[0]) < 1.0f)
            path.erase(path.begin());
    }

    Component::Update();
}

void CharacterMovement::AIUpdate() {
    if (path.empty() && !isAlarmed) {
        SetNewRandomPoint();
        SetNewPathWithPathFinding();
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

void CharacterMovement::SetNewPathToPlayer(glm::vec3 playerPosition) {
    previousTarget = endTarget;
    endTarget = playerPosition;
    isAlarmed = true;
    speedMultiplier = 2.0f;
    SetNewPathWithPathFinding();
}

void CharacterMovement::ReturnToPreviousPath() {
    endTarget = previousTarget;
    isAlarmed = false;
    speedMultiplier = 1.0f;
    SetNewPathWithPathFinding();
}

void CharacterMovement::SetNewPathWithPathFinding() {
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

