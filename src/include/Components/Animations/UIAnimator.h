#ifndef GLOOMENGINE_UIANIMATOR_H
#define GLOOMENGINE_UIANIMATOR_H

#include <vector>
#include "Components/Component.h"
#include "glm/vec3.hpp"
#include "Components/UI/Image.h"

enum class AnimationBehaviour {OneTime, Looping, Resetable};
enum class AnimatedProperty { Position, Rotation, Scale, Color, Alpha };
struct AnimationCheckpoint { AnimatedProperty property; glm::vec3 value; float duration=1; };

class UIAnimator : public Component {
private:
    float counter;
    std::vector<AnimationCheckpoint> checkpoints;
    int checkpointIndex = 0;
    AnimationCheckpoint checkpoint;
    std::shared_ptr<UIComponent> uiComponent;
    glm::vec3 valueDelta;
    bool loopingAnimation;
    AnimationBehaviour type;

public:
    bool paused = false;

    UIAnimator(const std::shared_ptr<GameObject> &parent, int id);

    void Setup(std::shared_ptr<UIComponent> animatedImage,
               std::vector<AnimationCheckpoint> animation,
               AnimationBehaviour behaviour = AnimationBehaviour::OneTime);
    void Reset();

    void Update() override;

    void OnDestroy() override;

private:
    void CalcValueDelta();
};

#endif //GLOOMENGINE_UIANIMATOR_H
