//
// Created by Adrian on 05.04.2023.
//

#include "Components/Audio/AudioSource.h"
#include "EngineManagers/AudioManager.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"

AudioSource::AudioSource(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

AudioSource::~AudioSource() = default;

void AudioSource::OnCreate() {
    AudioManager::GetInstance()->audioSources.insert({id, std::dynamic_pointer_cast<AudioSource>(shared_from_this())});
    position = parent->transform->GetLocalPosition();
    Component::OnCreate();
}

void AudioSource::OnDestroy() {
    AudioManager::GetInstance()->RemoveAudioSource(id);
    Component::OnDestroy();
}

void AudioSource::OnUpdate() {
    position = parent->transform->GetLocalPosition();
    Component::OnUpdate();
}
