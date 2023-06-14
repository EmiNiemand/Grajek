//
// Created by Adrian on 05.04.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AudioManager.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Audio/AudioLoader.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

AudioSource::AudioSource(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

AudioSource::~AudioSource() = default;

void AudioSource::Start() {
    auto player = GloomEngine::GetInstance()->FindGameObjectWithName("Player");

    if (player != nullptr)
        playerPos = player->transform;

    if (startWithTheGame)
        alSourcePlay(sourceId);

    Component::Start();
}

void AudioSource::Update() {
#ifdef DEBUG
    ZoneScopedNC("Audio source", 0x800080);
#endif

    alGetSourcei(sourceId, AL_SOURCE_STATE, &currentState);

    if (maxDistance > 0.0f) {
        if (distanceMode == AudioDistanceMode::Paused) {
            if (glm::distance(playerPos->GetLocalPosition(), position) > maxDistance)
                PauseSound();
            else
                PlaySound();
        } else {
            if (glm::distance(playerPos->GetLocalPosition(), position) > maxDistance)
                alSourcef(sourceId, AL_GAIN, 0.0f);
            else
                alSourcef(sourceId, AL_GAIN, gain);
        }
    }

    alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &currentState);

    if (isPlaying && !isLooping)
        isPlaying = audioLoader->FillProcessedBuffers(currentState);
    else if (isLooping)
        audioLoader->FillProcessedBuffers(currentState);

    Component::Update();
}

void AudioSource::OnCreate() {
    AudioManager::GetInstance()->audioSources.insert({id, std::dynamic_pointer_cast<AudioSource>(shared_from_this())});
    audioLoader = std::make_shared<AudioLoader>();
    Component::OnCreate();
}

void AudioSource::OnDestroy() {
    AudioManager::GetInstance()->RemoveAudioSource(id);
    playerPos = nullptr;
    audioLoader->CloseFile();
    audioLoader = nullptr;
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
    alSourceStop(sourceId);
    alSourcei(sourceId, AL_BUFFER, NULL);
    alDeleteBuffers(4, buffersIds);
    alDeleteSources(1, &sourceId);
}

/**
 * @annotation
 * Sets audio to play when calling Start() method (e.g. after loading the game).
 * @param state - true or false
 */
void AudioSource::PlaySoundAfterStart(const bool &state) {
    startWithTheGame = state;
}

/**
 * @annotation
 * Loads audio. Requires path to file and sound type. NOTE: types are either placed in res/sounds/direct or
 * res/sounds/positional (indicating their purpose).
 * @param path - path to .wav file
 * @param type - Positional or Direct audio
 */
void AudioSource::LoadAudioData(const std::string& path, AudioType type) {
    alGenBuffers(4, buffersIds);
    alGenSources(1, &sourceId);

    alSourcef(sourceId, AL_PITCH, 1);
    alSourcef(sourceId, AL_GAIN, gain);

    if (type == AudioType::Positional) {
        position = parent->transform->GetLocalPosition() + positionOffset;

        alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
        alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);
        alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.5f);
    }

    alSourcei(sourceId, AL_LOOPING, AL_FALSE);

    audioLoader->InitializeAudioLoader(sourceId, buffersIds);
    audioLoader->OpenFile(path);
    audioLoader->LoadFileHeader(type);
    audioLoader->FillBuffersQueue();
}

/**
 * @annotation
 * Plays the sound only if it is not currently playing.
 */
void AudioSource::PlaySound() {
    if (currentState != AL_PLAYING) {
        isPlaying = true;
        alSourcePlay(sourceId);
    }
}

/**
 * @annotation
 * Plays the sound despite its current status.
 */
void AudioSource::ForcePlaySound() {
    isPlaying = true;
    alSourcePlay(sourceId);
}

/**
 * @annotation
 * Pauses the sound.
 */
void AudioSource::PauseSound() const {
    if (currentState == AL_PLAYING) {
        alSourcePause(sourceId);
    }
}

/**
 * @annotation
 * Stops the sound.
 */
void AudioSource::StopSound() const {
    if (currentState == AL_PLAYING)
        alSourceStop(sourceId);
}

/**
 * @annotation
 * Sets position offset.
 * @param offset - vector3 added to position, default {0}
 */
void AudioSource::SetPositionOffset(const glm::vec3& offset) {
    positionOffset = offset;
    position += positionOffset;
    alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
}

/**
 * @annotation
 * Sets audio distance mode. For Paused, audio will be paused until player is again in audio range. For Continuous,
 * audio will still play with zero volume until player is again in audio range.
 * @param mode - Paused or Continuous, default Continuous
 */
void AudioSource::SetDistanceMode(const AudioDistanceMode& mode) {
    distanceMode = mode;
}

/**
 * @annotation
 * Sets audio pitch.
 * @param val - [0.5 - 2.0], default 1.0
 */
void AudioSource::SetPitch(const float& val) const {
    alSourcef(sourceId, AL_PITCH, val);
}

/**
 * @annotation
 * Sets audio gain. Each division/multiply by 2 results in -6/+6 dB.
 * @param val - [0.0 - x], default 1.0.
 */
void AudioSource::SetGain(const float& val) {
    gain = std::clamp(val, 0.0f, 1.0f);

    alSourcef(sourceId, AL_GAIN, val);
}

/**
 * @annotation
 * Sets audio velocity. Used in calculating doppler shift (moving objects emitting sound).
 * @param velocity - vector3, default {0}
 */
void AudioSource::SetVelocity(const glm::vec3& velocity) const {
    alSource3f(sourceId, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

/**
 * @annotation
 * Sets max distance for playing audio. Audio volume will be adjusted to better suit new distance
 * for more realistic effect.
 * @param val - [0.0f - x], default 0.0f
 */
void AudioSource::SetMaxDistance(const float& val) {
    maxDistance = val;

    if (maxDistance == 0.0f) {
        alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.5f);
        alSourcef(sourceId, AL_MAX_DISTANCE, maxDistance);
    } else if (maxDistance > 0.0f) {
        alSourcef(sourceId, AL_ROLLOFF_FACTOR,
                  gain * (maxDistance * 2.0f) / (1.5f + gain * 2.0f));
        alSourcef(sourceId, AL_MAX_DISTANCE, maxDistance);
    }
}

/**
 * @annotation
 * Sets audio looping state.
 * @param state - true or false, default false
 */
void AudioSource::IsLooping(const bool& state) {
    isLooping = state;
}

/**
 * @annotation
 * Sets the target moving state. If set to true, then the position will be updated every time target moves.
 * @param state - true or false, default false
 */
// Sets if the target will be moving. If set to true, then the position
void AudioSource::IsMoving(const bool& state) {
    isMovingTarget = state;
}

/**
 * @annotation
 * Sets audio cone settings.
 * @param direction - vector3, sound direction
 * @param cone - vector2, inner angle and outer angle
 */
void AudioSource::SetCone(const glm::vec3& direction, const glm::vec2& cone) const {
    alSource3f(sourceId, AL_DIRECTION, direction.x, direction.y, direction.z);
    alSourcef(sourceId, AL_CONE_INNER_ANGLE, cone.x);
    alSourcef(sourceId, AL_CONE_OUTER_ANGLE, cone.y);
}

const float AudioSource::GetGain() const {
    return gain;
}
