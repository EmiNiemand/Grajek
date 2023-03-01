//
// Created by szymo on 08/11/2022.
//

#ifndef OPENGLGP_TRANSFORM_H
#define OPENGLGP_TRANSFORM_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Transform  {
protected:
    //Local space information
    glm::vec3 mPos = {0.0f, 0.0f, 0.0f };
    glm::vec3 mEulerRot = {0.0f, 0.0f, 0.0f }; //In degrees
    glm::vec3 mScale = {1.0f, 1.0f, 1.0f };

    //Global space information concatenate in matrix
    glm::mat4 mModelMatrix = glm::mat4(1.0f);

    //Dirty flag
    bool mIsDirty = true;

protected:
    glm::mat4 GetLocalModelMatrix();
public:
    Transform();

    void ComputeModelMatrix();
    void ComputeModelMatrix(const glm::mat4& parentGlobalModelMatrix);

    void SetLocalPosition(const glm::vec3& newPosition);
    void SetLocalRotation(const glm::vec3& newRotation);
    void SetLocalScale(const glm::vec3& newScale);

    [[nodiscard]] const glm::vec3& GetGlobalPosition() const;
    [[nodiscard]] const glm::vec3& GetLocalPosition() const;
    [[nodiscard]] const glm::vec3& GetLocalRotation() const;
    [[nodiscard]] const glm::vec3& GetLocalScale() const;
    [[nodiscard]] const glm::mat4& GetModelMatrix() const;
    [[nodiscard]] glm::vec3 GetRight() const;
    [[nodiscard]] glm::vec3 GetUp() const;
    [[nodiscard]] glm::vec3 GetBackward() const;
    [[nodiscard]] glm::vec3 GetForward() const;
    [[nodiscard]] glm::vec3 GetGlobalScale() const;

    bool IsDirty() const;
};


#endif //OPENGLGP_TRANSFORM_H
