//
// Created by Adrian on 07.04.2023.
//

#include <al.h>
#include <alc.h>
#include "Components/Audio/AudioListener.h"
#include "EngineManagers/AudioManager.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"

AudioListener::AudioListener(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

AudioListener::~AudioListener() = default;

void AudioListener::Start() {
    player = GloomEngine::GetInstance()->FindGameObjectWithName("Player");
    Component::Start();
}

void AudioListener::Update() {
    glm::vec3 playerPosition = player->transform->GetLocalPosition();
    alListener3f(AL_POSITION, playerPosition.x, playerPosition.y, playerPosition.z);
    Component::Update();
}

void AudioListener::OnCreate() {
    AudioManager::GetInstance()->audioListener = std::dynamic_pointer_cast<AudioListener>(shared_from_this());
    Component::OnCreate();
}