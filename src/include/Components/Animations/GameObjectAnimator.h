//
// Created by MasterKtos on 23.04.2023.
//

#ifndef GLOOMENGINE_GAMEOBJECTANIMATOR_H
#define GLOOMENGINE_GAMEOBJECTANIMATOR_H

#include <vector>
#include "Components/Component.h"
#include "glm/vec3.hpp"
#include "Components/Transform.h"
#include "Components/Animations/UIAnimator.h"

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
    GameObjectAnimator(const std::shared_ptr<GameObject> &parent, int id);

    void Setup(std::shared_ptr<Transform> animatedTransform,
               std::vector<AnimationCheckpoint> animation,
               bool looping = false, bool absoluteValues = false);
    void Update() override;
    void OnDestroy() override;
};


#endif //GLOOMENGINE_GAMEOBJECTANIMATOR_H
