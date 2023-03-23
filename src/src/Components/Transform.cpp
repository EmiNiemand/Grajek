#include "Components/Transform.h"
#include "GameObjectsAndPrefabs/GameObject.h"

Transform::Transform(const std::shared_ptr<GameObject> &parent) : parent(parent) {}

glm::mat4 Transform::GetLocalModelMatrix()
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(mEulerRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(mEulerRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(mEulerRot.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), mPos) * rotationMatrix * glm::scale(glm::mat4(1.0f), mScale);
}

void Transform::ComputeModelMatrix()
{
    mModelMatrix = GetLocalModelMatrix();
}

void Transform::ComputeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
{
    mModelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
}

void Transform::SetLocalPosition(const glm::vec3& newPosition)
{
    mPos = newPosition;
    parent->UpdateSelfAndChildren();
}

void Transform::SetLocalRotation(const glm::vec3& newRotation)
{
    mEulerRot = newRotation;
    parent->UpdateSelfAndChildren();
}

void Transform::SetLocalScale(const glm::vec3& newScale)
{
    mScale = newScale;
    parent->UpdateSelfAndChildren();
}

glm::vec3 Transform::GetGlobalPosition() const
{
    return mModelMatrix[3];
}

glm::vec3 Transform::GetLocalPosition() const
{
    return mPos;
}

glm::vec3 Transform::GetLocalRotation() const
{
    return mEulerRot;
}

glm::vec3 Transform::GetGlobalScale() const
{
    return {glm::length(GetRight()), glm::length(GetUp()), glm::length(GetBackward()) };
}

glm::vec3 Transform::GetLocalScale() const
{
    return mScale;
}

glm::mat4 Transform::GetModelMatrix() const
{
    return mModelMatrix;
}

glm::vec3 Transform::GetRight() const
{
    return mModelMatrix[0];
}


glm::vec3 Transform::GetUp() const
{
    return mModelMatrix[1];
}

glm::vec3 Transform::GetBackward() const
{
    return mModelMatrix[2];
}

glm::vec3 Transform::GetForward() const
{
    return -mModelMatrix[2];
}
