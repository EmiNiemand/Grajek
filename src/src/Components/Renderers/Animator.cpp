//
// Created by szymo on 27/03/2023.
//

#include "Components/Renderers/Animator.h"
#include "LowLevelClasses/Bone.h"

Animator::Animator(const std::shared_ptr<GameObject> &parent, int id, Animation *currentAnimation) : Component(parent, id),
                                                                                   currentAnimation(currentAnimation) {
    currentTime = 0.0;
    finalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++) finalBoneMatrices.emplace_back(1.0f);
}

Animator::~Animator() {}

void Animator::UpdateAnimation(float deltaTime) {
    deltaTime = deltaTime;
    if (currentAnimation)
    {
        currentTime += currentAnimation->GetTicksPerSecond() * deltaTime;
        currentTime = fmod(currentTime, currentAnimation->GetDuration());
        CalculateBoneTransform(&currentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}

void Animator::PlayAnimation(Animation* pAnimation) {
    currentAnimation = pAnimation;
    currentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* Bone = currentAnimation->FindBone(nodeName);

    if (Bone)
    {
        Bone->Update(currentTime);
        nodeTransform = Bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = currentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        finalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++) CalculateBoneTransform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
{
    return finalBoneMatrices;
}