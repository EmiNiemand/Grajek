//
// Created by szymo on 27/03/2023.
//

#ifndef GLOOMENGINE_BONE_H
#define GLOOMENGINE_BONE_H

#include "Other/GLMHelper.h"
#include "LowLevelClasses/Animation.h"

#include <glm/gtx/quaternion.hpp>
#include <vector>

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

class Bone
{
private:
    KeyPosition* positions;
    KeyRotation* rotations;
    unsigned int numPositions;
    unsigned int numRotations;

    glm::mat4 localTransform;
    std::string name;
    int ID;

public:
    Bone(std::string  name, int ID, const aiNodeAnim* channel);

    virtual ~Bone();

    void Update(float animationTime, float previousAnimationTime, float blendingTime, const Animation& previousAnimation);

    [[nodiscard]] const glm::mat4& GetLocalTransform() const;
    [[nodiscard]] const std::string& GetBoneName() const;
    [[nodiscard]] int GetBoneID() const;

    void SetId(int id);

    [[nodiscard]] int GetPositionIndex(float animationTime) const;
    [[nodiscard]] int GetRotationIndex(float animationTime) const;


private:
    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

    glm::vec3 InterpolatePosition(float animationTime, float previousAnimationTime, float blendingTime, Animation& previousAnimation);
    glm::mat4 InterpolateRotation(float animationTime, float previousAnimationTime, float blendingTime, Animation& previousAnimation);
};


#endif //GLOOMENGINE_BONE_H
