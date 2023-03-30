//
// Created by szymo on 27/03/2023.
//

#ifndef GLOOMENGINE_ANIMATOR_H
#define GLOOMENGINE_ANIMATOR_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <unordered_map>

#include "Components/Component.h"
#include "LowLevelClasses/Animation.h"

class Animator : public Component {
private:
    inline static std::unordered_map<uint32_t, std::shared_ptr<AnimationModel>> animationModels;
    inline static std::unordered_map<uint32_t, std::shared_ptr<Animation>> animations;

    std::vector<glm::mat4> finalBoneMatrices;
    std::shared_ptr<Animation> currentAnimation;
	std::shared_ptr<AnimationModel> model;
    float currentTime;
	bool isPlaying;

public:
    Animator(const std::shared_ptr<GameObject> &parent, int id);
    ~Animator() override;

    void LoadAnimationModel(std::string path);
	void LoadAnimation(std::string path);

	void Update() override;

	void UpdateAnimation(float deltaTime);
    void PlayAnimation(std::shared_ptr<Animation> pAnimation);
	void PauseAnimation();

    void Draw();
    std::vector<glm::mat4> GetFinalBoneMatrices();

private:
    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

};


#endif //GLOOMENGINE_ANIMATOR_H
