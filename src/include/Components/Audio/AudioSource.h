//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AUDIOSOURCE_H
#define GLOOMENGINE_AUDIOSOURCE_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include <al.h>
#include <vector>
#include <memory>
#include <unordered_map>

class GameObject;
class AudioListener;
class Transform;

class AudioSource : public Component {
    std::shared_ptr<Transform> playerPos;
    glm::vec3 position = {};
    glm::vec3 positionOffset = {};
    ALuint bufferId;
    ALuint sourceId;
    ALint currentState;
    float gain = 1.0f;
    float maxDistance = 0.0f;
    bool isMovingTarget = false;
    AudioDistanceMode distanceMode = AudioDistanceMode::Paused;

public:
    AudioSource(const std::shared_ptr<GameObject> &parent, int id);
    ~AudioSource() override;

    void Start() override;
    void Update() override;
    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;
    void Free();

    void LoadAudioData(const char* path, AudioType type);
    void PlaySound() const;
    void PlaySoundForce() const;
    void PauseSound() const;
    void StopSound() const;
    void SetPositionOffset(glm::vec3 offset);
    void SetDistanceMode(AudioDistanceMode mode);
    void SetGain(float val);
    void SetPitch(float val) const;
    void SetVelocity(glm::vec3 velocity) const;
    void SetMaxDistance(float val);
    void IsLooping(bool state) const;
    void IsMoving(bool state);
    void SetCone(glm::vec3 direction, glm::vec2 cone) const;

};

#endif //GLOOMENGINE_AUDIOSOURCE_H
