#ifndef GLOOMENGINE_ANIMATION_H
#define GLOOMENGINE_ANIMATION_H

#include "assimp/scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "AnimationModel.h"
#include <vector>

class Bone;

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
private:
    float duration;
    int ticksPerSecond;
    std::vector<Bone> bones;
    AssimpNodeData rootNode;
    std::map<std::string, BoneInfo> boneInfoMap;

public:
    Animation(const std::string& animationPath, AnimationModel* model);
    virtual ~Animation();

    Bone* FindBone(const std::string& name);


    int GetTicksPerSecond() const;
    float GetDuration() const;
    const AssimpNodeData& GetRootNode();
    const std::map<std::string,BoneInfo>& GetBoneIDMap();

private:
    void ReadMissingBones(const aiAnimation* animation, AnimationModel& model);
    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
};


#endif //GLOOMENGINE_ANIMATION_H
