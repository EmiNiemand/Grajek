//
// Created by szymo on 27/03/2023.
//

#ifndef GLOOMENGINE_ANIMATOR_H
#define GLOOMENGINE_ANIMATOR_H

#include "Components/Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "LowLevelClasses/Animation.h"

#include <vector>


class Animator : public Component {
private:
    std::vector<glm::mat4> finalBoneMatrices;
    Animation* currentAnimation;
    float currentTime;
    float deltaTime;

public:
    Animator(const std::shared_ptr<GameObject> &parent, int id, Animation *currentAnimation);

    ~Animator() override;

    void UpdateAnimation(float deltaTime);
    void PlayAnimation(Animation* pAnimation);

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
    std::vector<glm::mat4> GetFinalBoneMatrices();
};


#endif //GLOOMENGINE_ANIMATOR_H
