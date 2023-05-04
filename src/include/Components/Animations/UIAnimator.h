#ifndef GLOOMENGINE_UIANIMATOR_H
#define GLOOMENGINE_UIANIMATOR_H

#include <vector>
#include "Components/Component.h"
#include "glm/vec3.hpp"
#include "Components/UI/Image.h"

enum AnimatedProperty { Position, Rotation, Scale, Color, Alpha };
struct AnimationCheckpoint { AnimatedProperty property; glm::vec3 value; float duration=1; };

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
    UIAnimator(const std::shared_ptr<GameObject> &parent, int id);

    void Setup(std::shared_ptr<Image> animatedImage,
               std::vector<AnimationCheckpoint> animation,
               bool looping = false);

    void Update() override;
};

#endif //GLOOMENGINE_UIANIMATOR_H
