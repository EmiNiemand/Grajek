//
// Created by szymo on 27/03/2023.
//

#ifndef GLOOMENGINE_BONE_H
#define GLOOMENGINE_BONE_H

#include "Other/GLMHelper.h"

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

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};

class Bone
{
private:
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
    unsigned int numPositions;
    unsigned int numRotations;
    unsigned int numScales;

    glm::mat4 localTransform;
    std::string name;
    int ID;

public:
    Bone(std::string  name, int ID, const aiNodeAnim* channel);

    void Update(float animationTime);

    glm::mat4 GetLocalTransform();
    [[nodiscard]] const std::string& GetBoneName() const;
    [[nodiscard]] int GetBoneID() const;

    [[nodiscard]] int GetPositionIndex(float animationTime) const;
    [[nodiscard]] int GetRotationIndex(float animationTime) const;
    [[nodiscard]] int GetScaleIndex(float animationTime) const;


private:
    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

    glm::mat4 InterpolatePosition(float animationTime);
    glm::mat4 InterpolateRotation(float animationTime);
    glm::mat4 InterpolateScaling(float animationTime);
};


#endif //GLOOMENGINE_BONE_H
