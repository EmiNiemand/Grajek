#include "LowLevelClasses/Bone.h"
#include <utility>

Bone::Bone(std::string  name, int ID, const aiNodeAnim* channel) : name(std::move(name)), ID(ID), localTransform(1.0f)
{
    numPositions = channel->mNumPositionKeys;

    for (unsigned int positionIndex = 0; positionIndex < numPositions; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        auto timeStamp = (float)channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data{};
        data.position = GetGLMVec(aiPosition);
        data.timeStamp = timeStamp;
        positions.push_back(data);
    }

    numRotations = channel->mNumRotationKeys;
    for (unsigned int rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        auto timeStamp = (float)channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data{};
        data.orientation = GetGLMQuat(aiOrientation);
        data.timeStamp = timeStamp;
        rotations.push_back(data);
    }
}

void Bone::Update(float animationTime)
{
    glm::mat4 translation = InterpolatePosition(animationTime);
    glm::mat4 rotation = InterpolateRotation(animationTime);
    localTransform = translation * rotation;
}

glm::mat4 Bone::GetLocalTransform() {
    return localTransform;
}

const std::string& Bone::GetBoneName() const {
    return name;
}

int Bone::GetBoneID() const{
    return ID;
}

int Bone::GetPositionIndex(float animationTime) const {
    for (int index = 0; index < numPositions - 1; ++index)
    {
        if (animationTime < positions[index + 1].timeStamp)
            return index;
    }
    return -1;
}

int Bone::GetRotationIndex(float animationTime) const {
    for (int index = 0; index < numRotations - 1; ++index)
    {
        if (animationTime < rotations[index + 1].timeStamp)
            return index;
    }
    return -1;
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor;
    const float midWayLength = animationTime - lastTimeStamp;
    const float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
    if (numPositions == 1)
        return glm::translate(glm::mat4(1.0f), positions[0].position);

    int p0Index = GetPositionIndex(animationTime);
    if (p0Index == -1) {
        return glm::mat4(1);
    }
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp, positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(positions[p0Index].position, positions[p1Index].position, scaleFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
    if (numRotations == 1)
    {
        auto rotation = glm::normalize(rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    if (p0Index == -1) {
        return glm::mat4(1);
    }
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp, rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation, rotations[p1Index].orientation, scaleFactor);
    finalRotation = glm::normalize(finalRotation);

    return glm::toMat4(finalRotation);
}

void Bone::SetId(int id) {
    ID = id;
}
