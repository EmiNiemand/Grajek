//
// Created by szymo on 27/03/2023.
//

#include "Components/Renderers/Animator.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/Bone.h"
#include "EngineManagers/RendererManager.h"
#include "GloomEngine.h"
#include <filesystem>
#include <utility>

#define BASE_PATH "res/models/"

Animator::Animator(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {
    currentTime = 0.0;
    finalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++) finalBoneMatrices.emplace_back(1.0f);
}

Animator::~Animator() {}

void Animator::LoadAnimation(std::string path)
{
	std::filesystem::path normalizedPath(BASE_PATH + path);
	model = std::make_shared<AnimationModel>(
			normalizedPath.string().c_str(),
			RendererManager::GetInstance()->animatedShader);
	currentAnimation = std::make_shared<Animation>(
			normalizedPath.string(), model.get());
}


void Animator::Update() {
	Component::Update();
	UpdateAnimation(GloomEngine::GetInstance()->deltaTime);

	//TODO: improve (ugly)
	auto shader = RendererManager::GetInstance()->animatedShader;
	shader->Activate();
	for (int i = 0; i < finalBoneMatrices.size(); ++i)
		shader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", finalBoneMatrices[i]);
	//TODO: I don't even know man
	shader->SetMat4("model", parent->transform->GetModelMatrix());
	model->Draw();
}

void Animator::UpdateAnimation(float deltaTime) {
	//if(!isPlaying) return;
	if (!currentAnimation) return;

    currentTime += currentAnimation->GetTicksPerSecond() * deltaTime;
    currentTime = fmod(currentTime, currentAnimation->GetDuration());
    CalculateBoneTransform(&currentAnimation->GetRootNode(), glm::mat4(1.0f));
}

void Animator::PlayAnimation(std::shared_ptr<Animation> pAnimation) {
    currentAnimation = std::move(pAnimation);
    currentTime = 0.0f;
	isPlaying = true;
}

void Animator::PauseAnimation() { isPlaying = false; }

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

	Bone* bone = currentAnimation->FindBone(nodeName);

    if (bone)
    {
	    bone->Update(currentTime);
        nodeTransform = bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = currentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        finalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
{
    return finalBoneMatrices;
}
