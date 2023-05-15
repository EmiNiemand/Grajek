#include "Components/Renderers/Animator.h"
#include "GloomEngine.h"
#include "Utilities.h"
#include "EngineManagers/RendererManager.h"
#include "EngineManagers/AnimationManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/Bone.h"
#include "Other/FrustumCulling.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <filesystem>
#include <utility>
#include <unordered_set>


#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

Animator::Animator(const std::shared_ptr<GameObject> &parent, int id) : Drawable(parent, id) {
    currentTime = 0.0;

    for (int i = 0; i < BONE_NUMBER; i++) finalBoneMatrices[i] = glm::mat4(1.0f);
}

Animator::~Animator() {
    model.reset();
}

void Animator::LoadAnimationModel(const std::string& path) {
    std::string newPath = "res/models/" + path;
    const std::filesystem::path normalizedPath(newPath);
    const int hash = Utilities::Hash(newPath);

    if (!animationModels.contains(hash)) {
        animationModels.insert({hash, std::make_shared<AnimationModel>( normalizedPath.string(),
                                                               RendererManager::GetInstance()->shader)});
    }

    model = animationModels.at(hash);

    parent->bounds = FrustumCulling::GenerateAABB(model);
}

void Animator::LoadAnimation(const std::string& path)
{
    if (animationModels.empty()) LoadModel(path);

    auto model = animationModels.begin()->second;

    std::string newPath = "res/models/" + path;
    std::filesystem::path normalizedPath(newPath);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(newPath, aiProcess_LimitBoneWeights);
    assert(scene && scene->mRootNode);
    for (int i = 0; i < scene->mNumAnimations; i++){
        aiAnimation* animation = scene->mAnimations[i];

        const int hash = Utilities::Hash(path);

        if (!animations.contains(hash)) {
            animations.insert({hash, Animation(path, (float)animation->mDuration, (int)animation->mTicksPerSecond)});
        }

        animations.at(hash).ReadHierarchyData(animations.at(hash).rootNode, scene->mRootNode);
        animations.at(hash).ReadMissingBones(animation, model);
    }
}

void Animator::SetAnimation(const std::string &name) {
    currentAnimation = animations.at(Utilities::Hash(name));

    currentAnimation.Recalculate(model);
}

void Animator::Update() {
#ifdef DEBUG
    ZoneScopedNC("Animator", 0x800080);
#endif
    if (!parent->isOnFrustum) {
        return;
    }

    AnimationManager::GetInstance()->AddToBuffer(std::dynamic_pointer_cast<Animator>(shared_from_this()));

    Drawable::Update();
}

void Animator::Draw() {
    if(model == nullptr) return;

    auto shader = RendererManager::GetInstance()->shader;

    shader->Activate();
    for (int i = 0; i < BONE_NUMBER; ++i)
        shader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", finalBoneMatrices[i]);
    shader->SetMat4("model", parent->transform->GetModelMatrix());
    shader->SetVec2("texStrech", textScale);
    shader->SetVec3("material.color", material.color);
    shader->SetFloat("material.shininess", material.shininess);
    shader->SetFloat("material.reflection", material.reflection);
    shader->SetFloat("material.refraction", material.refraction);
    shader->SetBool("isAnimated", true);

    model->Draw();
}

void Animator::Draw(std::shared_ptr<Shader> shader) {
    if(model == nullptr) return;

    shader->Activate();
    for (int i = 0; i < BONE_NUMBER; ++i)
        shader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", finalBoneMatrices[i]);
    shader->SetMat4("model", parent->transform->GetModelMatrix());
    shader->SetVec2("texStrech", textScale);
    shader->SetVec3("material.color", material.color);
    shader->SetFloat("material.shininess", material.shininess);
    shader->SetFloat("material.reflection", material.reflection);
    shader->SetFloat("material.refraction", material.refraction);
    shader->SetBool("isAnimated", true);

    model->Draw(shader);
}


void Animator::UpdateAnimation(float deltaTime) {
    currentTime += (float)currentAnimation.GetTicksPerSecond() * deltaTime;
    currentTime = fmod(currentTime, currentAnimation.GetDuration());
    CalculateBoneTransform(&currentAnimation.GetRootNode(), glm::mat4(1.0f));
}

void Animator::PlayAnimation(Animation pAnimation) {
    currentAnimation = pAnimation;
    currentTime = 0.0f;
	isPlaying = true;
}

void Animator::PauseAnimation() { isPlaying = false; }

glm::mat4* Animator::GetFinalBoneMatrices() {
    return finalBoneMatrices;
}

void Animator::CalculateBoneTransform(AssimpNodeData* node, const glm::mat4& parentTransform) {
//#ifdef DEBUG
//    ZoneScopedNC("CBT", 0xDC143C);
//#endif
    auto boneInfoMap = currentAnimation.GetBoneIDMap();

    const int nodeNumber = currentAnimation.nodeCounter;

    auto matrices = new glm::mat4[nodeNumber];
    auto toVisit = new std::pair<int, AssimpNodeData *>[nodeNumber];

    glm::mat4 nodeTransform;
    glm::mat4 globalTransformation;

    int iterator = 0;

    std::string* nodeName;
    std::shared_ptr<Bone> bone;

    matrices[iterator] = parentTransform;

    toVisit[iterator] = std::make_pair(iterator, node);

    for (int i = 0; i < nodeNumber; ++i) {
        node = toVisit[i].second;

        nodeName = &node->name;
        nodeTransform = node->transformation;
        bone = currentAnimation.FindBone(*nodeName);

        if (bone) {
            bone->Update(currentTime);
            nodeTransform = bone->GetLocalTransform();
        }

        globalTransformation = matrices[toVisit[i].first] * nodeTransform;
        matrices[i] = globalTransformation;

        if (bone)
            finalBoneMatrices[boneInfoMap[*nodeName].id] = globalTransformation * boneInfoMap[*nodeName].offset;

        for (int j = 0; j < node->children.size(); j++) {
            iterator++;
            toVisit[iterator] = std::make_pair(i, &node->children[j]);
        }
    }

    delete[] matrices;
    delete[] toVisit;
}

void Animator::LoadModel(const std::string &path) {
    std::string newPath = "res/models/" + path;
    std::filesystem::path normalizedPath(newPath);
    int hash = Utilities::Hash(newPath);

    if (!animationModels.contains(hash)) {
        animationModels.insert({hash, std::make_shared<AnimationModel>( normalizedPath.string(),
                                                                        RendererManager::GetInstance()->shader)});
    }
}
