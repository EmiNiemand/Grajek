//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AUDIOSOURCE_H
#define GLOOMENGINE_AUDIOSOURCE_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include <memory>
#include <al.h>

constexpr int NUM_BUFFERS = 4;

class GameObject;
class AudioLoader;
class Transform;

class AudioSource : public Component {
    // Player position
    std::shared_ptr<Transform> playerPos = nullptr;
    glm::vec3 position = {};
    glm::vec3 positionOffset = {};
    glm::vec3 audioVelocity = {};
    // Audio data
    std::shared_ptr<AudioLoader> audioLoader = nullptr;
    ALuint buffersIds[NUM_BUFFERS] {};
    ALuint sourceId = 0;
    ALint currentState = 0;
    bool isLooping = false;
    bool isEndOfFile = false;
    bool startWithTheGame = false;
    // Settings
    float gain = 1.0f;
    float maxDistance = 0.0f;
    float pitch = 1.0f;
    bool isMovingTarget = false;
    AudioDistanceMode distanceMode = AudioDistanceMode::Continuous;
    AudioType audioType = AudioType::Direct;

public:
    AudioSource(const std::shared_ptr<GameObject> &parent, int id);
    ~AudioSource() override;

    void Awake() override;
    void Start() override;
    void Update() override;
    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;
    void Free();

    void PlaySoundAfterStart(const bool& state);
    void LoadAudioData(const std::string& path, AudioType type);
    void PlaySound() const;
    void ForcePlaySound() const;
    void PauseSound() const;
    void StopSound();
    void SetPositionOffset(const glm::vec3& offset);
    void SetDistanceMode(const AudioDistanceMode& mode);
    void SetGain(const float& val);
    void SetPitch(const float& val);
    void SetVelocity(const glm::vec3& velocity);
    void SetMaxDistance(const float& val);
    void IsLooping(const bool& state);
    void IsMoving(const bool& state);
    void SetCone(const glm::vec3& direction, const glm::vec2& cone) const;
    const float GetGain() const;

};

#endif //GLOOMENGINE_AUDIOSOURCE_H
