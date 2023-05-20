#include "LowLevelClasses/Bone.h"
#include "GloomEngine.h"
#include <utility>

Bone::Bone(std::string  name, int ID, const aiNodeAnim* channel) : name(std::move(name)), ID(ID), localTransform(1.0f)
{
    numPositions = channel->mNumPositionKeys;
    positions = new KeyPosition[numPositions];

    for (unsigned int positionIndex = 0; positionIndex < numPositions; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        auto timeStamp = (float)channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data{};
        data.position = GetGLMVec(aiPosition);
        data.timeStamp = timeStamp;
        positions[positionIndex] = data;
    }

    numRotations = channel->mNumRotationKeys;
    rotations = new KeyRotation[numRotations];

    for (unsigned int rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        auto timeStamp = (float)channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data{};
        data.orientation = GetGLMQuat(aiOrientation);
        data.timeStamp = timeStamp;
        rotations[rotationIndex] = data;
    }
}

Bone::~Bone() {
    delete[] positions;
    delete[] rotations;
}


void Bone::Update(float animationTime, float previousAnimationTime, float blendingTime, const Animation& previousAnimation)
{
    glm::vec4 translation = glm::vec4(InterpolatePosition(animationTime, previousAnimationTime, blendingTime,
                                                          const_cast<Animation &>(previousAnimation)), 1);
    glm::mat4 rotation = InterpolateRotation(animationTime, previousAnimationTime, blendingTime,
                                             const_cast<Animation &>(previousAnimation));
    rotation[3] = translation;
    localTransform = rotation;
}

const glm::mat4& Bone::GetLocalTransform() const {
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
    if (lastTimeStamp + nextTimeStamp == 0)
        return abs(animationTime / 0.0001f);
    return abs(animationTime / (nextTimeStamp + lastTimeStamp));
}

glm::vec3 Bone::InterpolatePosition(float animationTime, float previousAnimationTime, float blendingTime, Animation& previousAnimation)
{
    if (numPositions == 1)
        return positions[0].position;

    int p0Index = GetPositionIndex(animationTime);
    if (p0Index == -1) return glm::vec3(1);
    if (animationTime <= 0) {
        float scaleFactor = GetScaleFactor(blendingTime, positions[p0Index].timeStamp, animationTime);
        auto bone = previousAnimation.FindBone(name);
        if (bone) {
            const glm::vec3 finalPosition = glm::mix(bone ->positions[GetPositionIndex(previousAnimationTime)].position,
                                                     positions[p0Index].position, 1 - scaleFactor);
            return finalPosition;
        }
        return glm::vec3(1);
    }
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp, positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(positions[p0Index].position, positions[p1Index].position, scaleFactor);
    return finalPosition;
}

glm::mat4 Bone::InterpolateRotation(float animationTime, float previousAnimationTime, float blendingTime, Animation& previousAnimation)
{
    if (numRotations == 1)
    {
        auto rotation = glm::normalize(rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    if (p0Index == -1) return glm::mat4(1);
    if (animationTime <= 0) {
        float scaleFactor = GetScaleFactor(blendingTime, rotations[p0Index].timeStamp, animationTime);
        auto bone = previousAnimation.FindBone(name);
        if (bone) {
            glm::quat finalRotation = glm::slerp(bone->rotations[GetPositionIndex(previousAnimationTime)].orientation,
                                                 rotations[p0Index].orientation, 1 - scaleFactor);
            finalRotation = glm::normalize(finalRotation);
            return glm::toMat4(finalRotation);
        }
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