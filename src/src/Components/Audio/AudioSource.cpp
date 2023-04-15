//
// Created by Adrian on 05.04.2023.
//

#include <iostream>
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Audio/AudioSource.h"
#include "EngineManagers/AudioManager.h"
#include "LowLevelClasses/AudioLoader.h"

AudioSource::AudioSource(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

AudioSource::~AudioSource() = default;

void AudioSource::Start() {
    playerPos = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform;
    Component::Start();
}

void AudioSource::Update() {
    alGetSourcei(sourceId, AL_SOURCE_STATE, &currentState);

    if (maxDistance >= 0.0) {
        if (distanceMode == AudioDistanceMode::Paused) {
            if (glm::distance(playerPos->GetLocalPosition(), position) > maxDistance) {
                PauseSound();
            } else {
                PlaySound();
            }
        } else {
            if (glm::distance(playerPos->GetLocalPosition(), position) > maxDistance) {
                alSourcef(sourceId, AL_GAIN, 0.0f);
            } else {
                alSourcef(sourceId, AL_GAIN, gain);
            }
        }
    }

    Component::Update();
}

void AudioSource::OnCreate() {
    AudioManager::GetInstance()->audioSources.insert({id, std::dynamic_pointer_cast<AudioSource>(shared_from_this())});
    Component::OnCreate();
}

void AudioSource::OnDestroy() {
    AudioManager::GetInstance()->RemoveAudioSource(id);
    Component::OnDestroy();
}

void AudioSource::OnUpdate() {
    if (isMovingTarget) {
        position = parent->transform->GetLocalPosition() + positionOffset;
        alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
    }
    Component::OnUpdate();
}

void AudioSource::Free() {
    alDeleteSources(1, &sourceId);
    alDeleteBuffers(1, &bufferId);
}

// Loads audio. Requires path to file and sound type.
// NOTE: type is either placed in res/sounds as a Sound or in /res/music as a Music.
// Sound offers 3D positioning and should be a Mono format.
// Music DOES NOT offer 3D positioning and should be a Stereo format.
void AudioSource::LoadAudioData(const char* path, AudioType type) {
    std::unique_ptr<AudioLoader> loader = std::make_unique<AudioLoader>();

    alGenBuffers(1, &bufferId);

    //  Currently unused, maybe in future for audio streaming
    //  AudioManager::GetInstance()->audioBuffers.insert({bufferId, std::dynamic_pointer_cast<AudioSource>(shared_from_this())});

    loader->LoadAudioFile(path, type, bufferId);

    alGenSources(1, &sourceId);

    alSourcef(sourceId, AL_PITCH, 1);
    alSourcef(sourceId, AL_GAIN, gain);

    if (type == AudioType::Sound) {
        position = parent->transform->GetLocalPosition() + positionOffset;
std::cout << position.x << " " << position .y << " " << position.z << std::endl;
        alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
        alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);
        alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.5f);
    }

    alSourcei(sourceId, AL_LOOPING, AL_FALSE);
    alSourcei(sourceId, AL_BUFFER, bufferId);
}

// Plays the sound only if it is not currently playing
void AudioSource::PlaySound() const {
    if (currentState != AL_PLAYING) {
        alSourcePlay(sourceId);
    }
}

// Force plays the sound
void AudioSource::ForcePlaySound() const {
    alSourcePlay(sourceId);
}

// Pauses the sound
void AudioSource::PauseSound() const {
    if (currentState == AL_PLAYING) {
        alSourcePause(sourceId);
    }
}

// Stops the sound (useful for resets)
void AudioSource::StopSound() const {
    if (currentState == AL_PLAYING) {
        alSourceStop(sourceId);
    }
}

// Sets offset which will be added to the current
// position. Type: glm::vec3, default {0, 0, 0}
void AudioSource::SetPositionOffset(glm::vec3 offset) {
    positionOffset = offset;
    position += positionOffset;
    alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
}

// Sets audio distance mode. 
// If set to Paused, audio will be paused
// until player is again in audio range.
// If set to Continuous, audio will still play with 0 volume
// until player is again in audio range.
void AudioSource::SetDistanceMode(AudioDistanceMode mode) {
    distanceMode = mode;
}

// Sets audio pitch. Type: flaot [0.5 - 2.0], default 1.0
void AudioSource::SetPitch(float val) const {
    alSourcef(sourceId, AL_PITCH, val);
}

// Sets audio gain. Type: flaot [0.0 - x], default 1.0.
// Each division/multiply by 2 results in -6/+6 dB
void AudioSource::SetGain(float val) {
    gain = val;
    alSourcef(sourceId, AL_GAIN, val);
}

// Sets audio velocity. Type: glm::vec3, default {0, 0, 0}
// Used in calculating doppler shift (moving objects emitting sound)
void AudioSource::SetVelocity(glm::vec3 velocity) const {
    alSource3f(sourceId, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

// Sets max distance for playing audio. Type: float, default 0.0f
void AudioSource::SetMaxDistance(float val) {
    maxDistance = val;

    if (val == 0.0f) {
        alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.5f);
    } else {
        alSourcef(sourceId, AL_ROLLOFF_FACTOR,
                  gain * (maxDistance * 2.0f - 1.0f) / (1.5f + gain * 2.0f));
    }
}

// Sets audio looping state. Type: bool, default false
void AudioSource::IsLooping(bool state) const {
    alSourcei(sourceId, AL_LOOPING, state);
}

// Sets if the target will be moving. If set to true, then the position
// will be updated every time target moves. Type: bool, default false
void AudioSource::IsMoving(bool state) {
    isMovingTarget = state;
}

// Sets audio cone settings. Type: glm::vec3.
// First is a sound direction, second are cone settings (inner angle, outer angle).
// NOTE: Outer angle is basically 360.0f - Inner angle.
void AudioSource::SetCone(glm::vec3 direction, glm::vec2 cone) const {
    alSource3f(sourceId, AL_DIRECTION, direction.x, direction.y, direction.z);
    alSourcef(sourceId, AL_CONE_INNER_ANGLE, cone.x);
    alSourcef(sourceId, AL_CONE_OUTER_ANGLE, cone.y);
}
