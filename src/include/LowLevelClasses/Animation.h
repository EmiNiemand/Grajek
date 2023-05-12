#ifndef GLOOMENGINE_ANIMATION_H
#define GLOOMENGINE_ANIMATION_H

#include "assimp/scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "AnimationModel.h"
#include <vector>
#include <unordered_map>

class Bone;

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    std::vector<AssimpNodeData> children;
};

class Animation
{
public:
    std::string name;
    float duration{};
    int ticksPerSecond{};
    std::unordered_map<std::string, std::shared_ptr<Bone>> bones;
    AssimpNodeData rootNode;
    int nodeCounter = 0;
    std::unordered_map<std::string, BoneInfo> boneInfoMap;

public:
    Animation();
    Animation(std::string name, float mDuration, int mTicksPerSecond);
    virtual ~Animation();

	const std::shared_ptr<Bone>& FindBone(const std::string& name);


    [[nodiscard]] int GetTicksPerSecond() const;
    [[nodiscard]] float GetDuration() const;
    AssimpNodeData& GetRootNode();
    const std::unordered_map<std::string, BoneInfo>& GetBoneIDMap();

    void ReadMissingBones(const aiAnimation* animation, const std::shared_ptr<AnimationModel>& model);
    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
    void Recalculate(const std::shared_ptr<AnimationModel>& model);
};


#endif //GLOOMENGINE_ANIMATION_H
