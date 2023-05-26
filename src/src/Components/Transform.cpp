#include "Components/Transform.h"
#include "GameObjectsAndPrefabs/GameObject.h"

Transform::Transform(const std::shared_ptr<GameObject> &parent) : parent(parent) {
    if (parent != nullptr) {
        parent->globalRotation = parent->globalRotation + GetLocalRotation();
    }
}

glm::mat4 Transform::GetLocalModelMatrix()
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(mEulerRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(mEulerRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(mEulerRot.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also known as TRS matrix)
    return glm::translate(glm::mat4(1.0f), mPos) * rotationMatrix * glm::scale(glm::mat4(1.0f), mScale);
}

void Transform::ComputeModelMatrix()
{
    mModelMatrix = GetLocalModelMatrix();
    parent->dirtyFlag = false;
}

void Transform::ComputeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
{
    mModelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
    parent->dirtyFlag = false;
}

void Transform::SetLocalPosition(const glm::vec3& newPosition)
{
    mPos = newPosition;
    if (!parent) return;
    if (parent->dirtyFlag) return;
    SetDirtyFlag();
}

void Transform::SetLocalRotation(const glm::vec3& newRotation)
{
    mEulerRot = newRotation;
    if (!parent) return;
    parent->RecalculateGlobalRotation();
    if (parent->dirtyFlag) return;
    SetDirtyFlag();
}

void Transform::SetLocalScale(const glm::vec3& newScale)
{
    mScale = newScale;
    if (!parent) return;
    if (parent->dirtyFlag) return;
    SetDirtyFlag();
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

void Transform::SetDirtyFlag() {
    const int gameObjectsSize = (int)GloomEngine::GetInstance()->gameObjects.size() + 1;
    auto toSet = new std::shared_ptr<GameObject>[gameObjectsSize];

    int checkIterator = 1;
    toSet[0] = parent;

    for (int i = 0; i < checkIterator; i++) {
        toSet[i]->dirtyFlag = true;
        for (const auto& child : toSet[i]->children) {
            toSet[checkIterator] = child.second;
            checkIterator++;
        }
    }

    delete[] toSet;
}
