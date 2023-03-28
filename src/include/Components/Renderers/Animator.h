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
    std::shared_ptr<Animation> currentAnimation;
	std::shared_ptr<AnimationModel> model;
    float currentTime;
	bool isPlaying;

public:
	glm::mat4 modelMatrix;

    Animator(const std::shared_ptr<GameObject> &parent, int id);
    ~Animator() override;

	void LoadAnimation(std::string path);

	void Update() override;

	void UpdateAnimation(float deltaTime);
    void PlayAnimation(std::shared_ptr<Animation> pAnimation);
	void PauseAnimation();

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
    std::vector<glm::mat4> GetFinalBoneMatrices();
};


#endif //GLOOMENGINE_ANIMATOR_H
