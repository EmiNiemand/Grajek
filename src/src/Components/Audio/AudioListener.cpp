//
// Created by Adrian on 07.04.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AudioManager.h"
#include "Components/Audio/AudioListener.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

AudioListener::AudioListener(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

AudioListener::~AudioListener() = default;

void AudioListener::Start() {
    position = parent->transform->GetLocalPosition();

    alListenerf(AL_GAIN, gain);
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);

    // "up" and "at" vectors, i.e. at the head of the player
    ALfloat listenerOrientation[] = {0.0, 0.0, -1.0,  0.0, 1.0, 0.0};
    alListenerfv(AL_ORIENTATION,listenerOrientation);

    Component::Start();
}

void AudioListener::Update() {
#ifdef DEBUG
    ZoneScopedNC("AudioListener", 0x800080);
#endif

    position = parent->transform->GetLocalPosition();
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    Component::Update();
}

void AudioListener::OnCreate() {
    AudioManager::GetInstance()->audioListener = std::dynamic_pointer_cast<AudioListener>(shared_from_this());
    Component::OnCreate();
}

/**
 * @annotation
 * Sets player audio gain (e.g. global volume level).
 * @param val - value between 0.0f - 1.0f
 */
void AudioListener::SetGain(const float& val) {
    gain = std::clamp(val, 0.0f, 1.0f);

    alListenerf(AL_GAIN, gain);
}

/**
 * @annotation
 * Returns current audio gain (e.g. global volume level).
 * @returns float - value between 0.0f - 1.0f
 */
const float AudioListener::GetGain() const {
    return gain;
}

/**
 * @annotation
 * Sets player velocity (for doppler shift).
 * @param vel - velocity
 */
void AudioListener::SetVelocity(const glm::vec3& vel) {
    velocity = vel;
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}
