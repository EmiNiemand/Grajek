#include "Components/Renderers/Animator.h"
#include "GloomEngine.h"
#include "Utilities.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/Bone.h"
#include <filesystem>
#include <utility>

Animator::Animator(const std::shared_ptr<GameObject> &parent, int id) : Drawable(parent, id) {
    currentTime = 0.0;
    finalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++) finalBoneMatrices.emplace_back(1.0f);
}

Animator::~Animator() {
    model.reset();
    currentAnimation.reset();
}

void Animator::LoadAnimationModel(std::string path) {
    std::string newPath = "res/models/" + path;
    std::filesystem::path normalizedPath(newPath);
    uint32_t hash = Utilities::Hash(newPath);

    if (!animationModels.contains(hash)) {
        animationModels.insert({hash, std::make_shared<AnimationModel>( normalizedPath.string(),
                                                               RendererManager::GetInstance()->animatedShader)});
    }
    model = animationModels.at(hash);
}

void Animator::LoadAnimation(std::string path)
{
    std::string newPath = "res/models/" + path;
    std::filesystem::path normalizedPath(newPath);
    uint32_t hash = Utilities::Hash(newPath);

    if (model == nullptr) LoadAnimationModel(path);

    if (!animations.contains(hash)) {
        animations.insert({hash, std::make_shared<Animation>(normalizedPath.string(), model.get())});
    }

	currentAnimation = animations.at(hash);
}


void Animator::Update() {
	UpdateAnimation(GloomEngine::GetInstance()->deltaTime);
    Drawable::Update();
}

void Animator::Draw() {
    if(model == nullptr) return;

    auto shader = RendererManager::GetInstance()->animatedShader;

    shader->Activate();
    for (int i = 0; i < finalBoneMatrices.size(); ++i)
        shader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", finalBoneMatrices[i]);
    shader->SetMat4("model", parent->transform->GetModelMatrix());
    shader->SetVec3("material.color", material.color);
    shader->SetFloat("material.shininess", material.shininess);
    shader->SetFloat("material.reflection", material.reflection);
    shader->SetFloat("material.refraction", material.refraction);

    model->Draw();
}

void Animator::Draw(std::shared_ptr<Shader> shader) {
    if(model == nullptr) return;

    shader->Activate();
    for (int i = 0; i < finalBoneMatrices.size(); ++i)
        shader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", finalBoneMatrices[i]);
    shader->SetMat4("model", parent->transform->GetModelMatrix());
    shader->SetVec3("material.color", material.color);
    shader->SetFloat("material.shininess", material.shininess);
    shader->SetFloat("material.reflection", material.reflection);
    shader->SetFloat("material.refraction", material.refraction);

    model->Draw(shader);
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

std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
{
    return finalBoneMatrices;
}

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