//
// Created by MasterKtos on 23.04.2023.
//

#ifndef GLOOMENGINE_GAMEOBJECTANIMATOR_H
#define GLOOMENGINE_GAMEOBJECTANIMATOR_H

#include <vector>
#include "Components/Component.h"
#include "glm/vec3.hpp"
#include "Components/Transform.h"

enum AnimatedProperty { Position, Rotation, Scale };
struct AnimationCheckpoint { AnimatedProperty property; glm::vec3 value; float duration=1; };

/**
 * @attention GameObjectAnimator has to be on separate GameObject
 */
class GameObjectAnimator : public Component {
private:
    float counter;
    std::vector<AnimationCheckpoint> checkpoints;
    int checkpointIndex = 0;
    AnimationCheckpoint checkpoint;
    std::shared_ptr<Transform> transform;
    glm::vec3 valueDelta;
    bool loopingAnimation;
    bool useAbsoluteValues;

public:
    GameObjectAnimator(const std::shared_ptr<GameObject> &parent, uint32_t id);

    void Setup(std::shared_ptr<Transform> animatedTransform,
               std::vector<AnimationCheckpoint> animation,
               bool looping = false, bool absoluteValues = false);
    void Update() override;
};


#endif //GLOOMENGINE_GAMEOBJECTANIMATOR_H
