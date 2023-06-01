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
    // Audio data
    std::shared_ptr<AudioLoader> audioLoader = nullptr;
    ALuint buffersIds[NUM_BUFFERS] {};
    ALuint sourceId = 0;
    ALint currentState = 0;
    bool isLooping = false;
    bool isPlaying = false;
    // Settings
    float gain = 1.0f;
    float maxDistance = 0.0f;
    bool isMovingTarget = false;
    AudioDistanceMode distanceMode = AudioDistanceMode::Continuous;

public:
    AudioSource(const std::shared_ptr<GameObject> &parent, int id);
    ~AudioSource() override;

    void Start() override;
    void Update() override;
    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;
    void Free();

    void LoadAudioData(const std::string& path, AudioType type);
    void PlaySound();
    void ForcePlaySound();
    void PauseSound() const;
    void StopSound() const;
    void SetPositionOffset(const glm::vec3& offset);
    void SetDistanceMode(const AudioDistanceMode& mode);
    void SetGain(const float& val);
    void SetPitch(const float& val) const;
    void SetVelocity(const glm::vec3& velocity) const;
    void SetMaxDistance(const float& val);
    void IsLooping(const bool& state);
    void IsMoving(const bool& state);
    void SetCone(const glm::vec3& direction, const glm::vec2& cone) const;

};

#endif //GLOOMENGINE_AUDIOSOURCE_H
