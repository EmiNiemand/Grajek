//
// Created by szymo on 27/03/2023.
//

#ifndef GLOOMENGINE_ANIMATOR_H
#define GLOOMENGINE_ANIMATOR_H

#include <vector>

#include "Drawable.h"
#include "LowLevelClasses/Animation.h"
#include "ProjectSettings.h"

class Animator : public Drawable {
private:
    glm::mat4 finalBoneMatrices[BONE_NUMBER];
	std::shared_ptr<AnimationModel> model;
    float currentTime;
	bool isPlaying = false;
    float blendingTimeInSeconds = 0.5f;
    float blendingTimeInTicks = 0;

public:
	float speed = 1;
    float previousAnimationTime = 0;
    Animation previousAnimation;
    Animation currentAnimation;

    inline static std::unordered_map<int, std::shared_ptr<AnimationModel>> animationModels;
    inline static std::unordered_map<int, Animation> animations;

public:
    Animator(const std::shared_ptr<GameObject> &parent, int id);
    ~Animator() override;

    void LoadAnimationModel(const std::string& path);
	static void LoadAnimation(const std::string& path);
    void SetAnimation(const std::string& name);

    void OnDestroy() override;
	void Update() override;
    void Draw() override;

    void Draw(std::shared_ptr<Shader> shader) override;

    void UpdateAnimation(float deltaTime);
    void PlayAnimation(Animation pAnimation);
	void PauseAnimation();

    glm::mat4* GetFinalBoneMatrices();

private:
    void CalculateBoneTransform(AssimpNodeData* node, const glm::mat4& parentTransform);
    static void LoadModel(const std::string& path);

};


#endif //GLOOMENGINE_ANIMATOR_H
