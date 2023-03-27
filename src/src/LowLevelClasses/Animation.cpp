#include "LowLevelClasses/Animation.h"
#include "LowLevelClasses/Bone.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "stb_image.h"
#include "spdlog/spdlog.h"

Animation::Animation(const std::string& animationPath, std::shared_ptr<AnimationModel> model)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	// TODO: error, scene is null
    assert(scene && scene->mRootNode);
    auto animation = scene->mAnimations[0];
    duration = animation->mDuration;
    ticksPerSecond = animation->mTicksPerSecond;
    aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
    globalTransformation = globalTransformation.Inverse();
    ReadHierarchyData(rootNode, *scene->mRootNode);
    ReadMissingBones(std::shared_ptr<aiAnimation>(animation), *model);
}

Animation::~Animation() {}

std::shared_ptr<Bone> Animation::FindBone(const std::string& name)
{
    auto iter = std::find_if(bones.begin(), bones.end(),
                             [&](const Bone& Bone)
                             {
                                 return Bone.GetBoneName() == name;
                             }
    );
    if (iter == bones.end()) return nullptr;
    else return std::shared_ptr<Bone>(&(*iter));
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

const std::map<std::string,BoneInfo>& Animation::GetBoneIDMap() {
    return boneInfoMap;
}

void Animation::ReadMissingBones(const std::shared_ptr<aiAnimation>& animation, AnimationModel& model) {
    int size = animation->mNumChannels;

    auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
    uint16_t& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        bones.push_back(Bone(channel->mNodeName.data,
                             boneInfoMap[channel->mNodeName.data].id, channel));
    }

    boneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, aiNode& src) {
	// TODO: this will probably cause problems, oh well
    //if(src == nullptr) return;

    dest.name = src.mName.data;
    dest.transformation = ConvertMatrixToGLMFormat(src.mTransformation);
    dest.childrenCount = src.mNumChildren;

    for (int i = 0; i < src.mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHierarchyData(newData, *src.mChildren[i]);
        dest.children.push_back(newData);
    }
}

