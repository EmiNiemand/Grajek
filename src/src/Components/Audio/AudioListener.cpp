//
// Created by Adrian on 07.04.2023.
//

#include "EngineManagers/AudioManager.h"
#include "Components/Audio/AudioListener.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

AudioListener::AudioListener() {
    alListenerf(AL_GAIN, gain);
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);

    // "up" and "at" vectors, i.e. at the head of the player
    ALfloat listenerOrientation[] = {0.0, 0.0, -1.0,  0.0, 1.0, 0.0};
    alListenerfv(AL_ORIENTATION,listenerOrientation);
}

AudioListener::~AudioListener() = default;

void AudioListener::UpdatePosition(const glm::vec3& position) {
#ifdef DEBUG
    ZoneScopedNC("AudioListener", 0x800080);
#endif

    alListener3f(AL_POSITION, position.x, position.y, position.z);
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
