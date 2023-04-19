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
private:
    float duration;
    int ticksPerSecond;
    std::vector<Bone> bones;
    AssimpNodeData rootNode;
    std::unordered_map<std::string, BoneInfo> boneInfoMap;

public:
    Animation(const std::string& animationPath, AnimationModel* model);
    virtual ~Animation();

	Bone* FindBone(const std::string& name);


    [[nodiscard]] int GetTicksPerSecond() const;
    [[nodiscard]] float GetDuration() const;
    const AssimpNodeData& GetRootNode();
    const std::unordered_map<std::string,BoneInfo>& GetBoneIDMap();

private:
    void ReadMissingBones(const aiAnimation* animation, AnimationModel& model);
    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
};


#endif //GLOOMENGINE_ANIMATION_H
