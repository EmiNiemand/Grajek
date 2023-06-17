//
// Created by szymo on 16/03/2023.
//

#include <numbers>
#include "Components/Scripts/Player/PlayerMovement.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/Audio/AudioSource.h"

PlayerMovement::PlayerMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

PlayerMovement::~PlayerMovement() = default;

void PlayerMovement::Start() {
    rb = parent->GetComponent<Rigidbody>();
    stepSound = parent->AddComponent<AudioSource>();
    stepSound->LoadAudioData("res/sounds/direct/walking_step.wav", AudioType::Direct);
    Component::Start();
}

void PlayerMovement::OnDestroy() {
    rb.reset();
    Component::OnDestroy();
}

void PlayerMovement::FixedUpdate() {
    Component::FixedUpdate();
    if (rb == nullptr)
        return;

    speed = std::lerp(speed, maxSpeed, smoothingParam);

    if (moveVector.x != 0.0f || moveVector.y != 0.0f) {
        rb->AddForce(glm::vec3(moveVector.x, 0.0f, moveVector.y) * speed, ForceMode::Force);
        isMoving = true;
    }

    if (!isMoving) {
        speed = 0.0f;
    } else {
        // Calculate rotation angles by using tangent function
        rotationAngle = std::atan2f(-moveVector.x, -moveVector.y) * 180.0f/std::numbers::pi;

        rb->AddTorque(rotationAngle, ForceMode::Force);
    }

    isMoving = false;
}

void PlayerMovement::Move(glm::vec2 inputVector) {
    // Need to invert horizontal/Z axis
    moveVector = inputVector * glm::vec2(1, -1);

    // Step sound
    if (moveVector != glm::vec2(0.0f)) {
        if (!stepSoundActive) {
            stepSound->PlaySound();
            stepSound->SetGain(0.5);
            stepSound->IsLooping(true);
        }
        stepSoundActive = true;
    } else {
        if (stepSoundActive) {
            stepSound->StopSound();
            stepSound->SetGain(0);
            stepSound->IsLooping(false);
        }
        stepSoundActive = false;
    }
}

void PlayerMovement::StopWalkingSound() {
    stepSound->StopSound();
    stepSound->SetGain(0);
    stepSound->IsLooping(false);
}
