#ifndef GLOOMENGINE_UIANIMATOR_H
#define GLOOMENGINE_UIANIMATOR_H

#include <vector>
#include "Components/Component.h"
#include "glm/vec3.hpp"
#include "GameObjectAnimator.h"
#include "Components/UI/Image.h"

class UIAnimator : public Component {
private:
    float counter;
    std::vector<AnimationCheckpoint> checkpoints;
    int checkpointIndex = 0;
    AnimationCheckpoint checkpoint;
    std::shared_ptr<Image> image;
    glm::vec3 valueDelta;
    bool loopingAnimation;

public:
    UIAnimator(const std::shared_ptr<GameObject> &parent, uint32_t id);

    void Setup(std::shared_ptr<Image> animatedImage,
               std::vector<AnimationCheckpoint> animation,
               bool looping = false);

    void Update() override;
};

#endif //GLOOMENGINE_UIANIMATOR_H
