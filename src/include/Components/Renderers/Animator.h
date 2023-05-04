//
// Created by szymo on 27/03/2023.
//

#ifndef GLOOMENGINE_ANIMATOR_H
#define GLOOMENGINE_ANIMATOR_H

#include <vector>

#include "Drawable.h"
#include "LowLevelClasses/Animation.h"

class Animator : public Drawable {
private:
    inline static std::unordered_map<uint32_t, std::shared_ptr<AnimationModel>> animationModels;
    inline static std::unordered_map<uint32_t, std::shared_ptr<Animation>> animations;

    std::vector<glm::mat4> finalBoneMatrices;
    std::shared_ptr<Animation> currentAnimation;
	std::shared_ptr<AnimationModel> model;
    float currentTime;
	bool isPlaying = false;

public:
    Animator(const std::shared_ptr<GameObject> &parent, uint32_t id);
    ~Animator() override;

    void LoadAnimationModel(const std::string& path);
	void LoadAnimations(const std::string& path);
    void SetAnimation(const std::string& name);

	void Update() override;
    void Draw() override;

    void Draw(std::shared_ptr<Shader> shader) override;

    void UpdateAnimation(float deltaTime);
    void PlayAnimation(std::shared_ptr<Animation> pAnimation);
	void PauseAnimation();

    std::vector<glm::mat4>& GetFinalBoneMatrices();

private:
    void CalculateBoneTransform(const AssimpNodeData* node, const glm::mat4& parentTransform);

};


#endif //GLOOMENGINE_ANIMATOR_H
