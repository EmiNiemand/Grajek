#include <utility>

#include "LowLevelClasses/Animation.h"
#include "LowLevelClasses/Bone.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "stb_image.h"
#include "spdlog/spdlog.h"

Animation::Animation() = default;

Animation::Animation(std::string name, float mDuration, int mTicksPerSecond) : name(std::move(name))
{
    bones.reserve(100);
    boneInfoMap.reserve(100);
    duration = mDuration;
    ticksPerSecond = mTicksPerSecond;
}

Animation::~Animation() = default;

std::shared_ptr<Bone> Animation::FindBone(const std::string& name)
{
    if (!bones.contains(name)) return nullptr;
    else return bones.at(name);
}

int Animation::GetTicksPerSecond() const {
    return ticksPerSecond;
}

float Animation::GetDuration() const {
    return duration;
}

AssimpNodeData& Animation::GetRootNode() {
    return rootNode;
}

const std::unordered_map<std::string, BoneInfo>& Animation::GetBoneIDMap() {
    return boneInfoMap;
}

void Animation::ReadMissingBones(const aiAnimation* animation, const std::shared_ptr<AnimationModel>& model) {
    unsigned int size = animation->mNumChannels;

    boneInfoMap = model->GetBoneInfoMap();//getting m_BoneInfoMap from Model class
    uint16_t& boneCount = model->GetBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (unsigned int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];

        int counter = 0;
        std::string boneName = channel->mNodeName.data;

        for (int j = 0; j < boneName.size(); j++) {
            if (boneName[j] == '_') counter++;
            if (counter == 2) {
                boneName = boneName.substr(j + 1, boneName.back());
                break;
            }
        }

        /// DO NOT TOUCH DOOPA
        if (counter == 1) {
            boneName = "doopa";
        }

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        bones.insert({boneName, std::make_shared<Bone>(boneName, boneInfoMap[boneName].id, channel)});
    }
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src) {
    assert(src);

    dest.name = src->mName.data;
    int counter = 0;

    for (int j = 0; j < dest.name.size(); j++) {
        if (dest.name[j] == '_') counter++;
        if (counter == 2) {
            dest.name = dest.name.substr(j + 1, dest.name.back());
            break;
        }
    }

    /// DO NOT TOUCH DOOPA
    if (counter == 1) {
        dest.name = "doopa";
    }

    dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
    dest.children.reserve(src->mNumChildren);

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

void Animation::Recalculate(const std::shared_ptr<AnimationModel>& model) {
    boneInfoMap = model->GetBoneInfoMap();
    uint16_t& boneCount = model->GetBoneCount();

    for (auto&& bone : bones) {
        std::string boneName = bone.second->GetBoneName();

        if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        bone.second->SetId(boneInfoMap[boneName].id);
    }
}

