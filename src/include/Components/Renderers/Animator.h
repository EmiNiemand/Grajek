//
// Created by szymo on 27/03/2023.
//

#ifndef GLOOMENGINE_ANIMATOR_H
#define GLOOMENGINE_ANIMATOR_H

#include <vector>

#include "Drawable.h"
#include "LowLevelClasses/Animation.h"

#define BONE_NUMBER 40

class Animator : public Drawable {
private:
    inline static std::unordered_map<int, std::shared_ptr<AnimationModel>> animationModels;
    inline static std::unordered_map<int, Animation> animations;

    glm::mat4 finalBoneMatrices[BONE_NUMBER];
    Animation currentAnimation;
	std::shared_ptr<AnimationModel> model;
    float currentTime;
	bool isPlaying = false;

public:
    Animator(const std::shared_ptr<GameObject> &parent, int id);
    ~Animator() override;

    void LoadAnimationModel(const std::string& path);
	static void LoadAnimation(const std::string& path);
    void SetAnimation(const std::string& name);

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
