#include "LowLevelClasses/Animation.h"
#include "LowLevelClasses/Bone.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "stb_image.h"
#include "spdlog/spdlog.h"

Animation::Animation(float mDuration, int mTicksPerSecond)
{
    bones.reserve(100);
    boneInfoMap.reserve(100);
    duration = mDuration;
    ticksPerSecond = mTicksPerSecond;
}

Animation::~Animation() = default;

Bone* Animation::FindBone(const std::string& name)
{
    auto iter = std::find_if(bones.begin(), bones.end(), [&](const Bone& Bone) { return Bone.GetBoneName() == name; });
    if (iter == bones.end()) return nullptr;
    else return &(*iter);
}

int Animation::GetTicksPerSecond() const {
    return ticksPerSecond;
}

float Animation::GetDuration() const {
    return duration;
}

const AssimpNodeData& Animation::GetRootNode() {
    return rootNode;
}

const std::unordered_map<std::string,BoneInfo>& Animation::GetBoneIDMap() {
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
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        bones.emplace_back(channel->mNodeName.data,
                             boneInfoMap[channel->mNodeName.data].id, channel);
    }
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src) {
    assert(src);

    dest.name = src->mName.data;
    dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
    dest.children.reserve(src->mNumChildren);

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

