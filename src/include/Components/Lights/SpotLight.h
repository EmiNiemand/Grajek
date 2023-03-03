//
// Created by szymo on 03/03/2023.
//

#ifndef OPENGLGP_SPOTLIGHT_H
#define OPENGLGP_SPOTLIGHT_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "include/Components/Component.h"

class SpotLight : public Component {
private:
    int id;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 color;

public:
    SpotLight(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id);
    ~SpotLight() override;

    void OnTransformUpdate() override;
    void OnRemove() override;

    float GetCutOff() const;
    void SetCutOff(float cutOff);

    float GetOuterCutOff() const;
    void SetOuterCutOff(float outerCutOff);

    float GetConstant() const;
    void SetConstant(float constant);

    float GetLinear() const;
    void SetLinear(float linear);

    float GetQuadratic() const;
    void SetQuadratic(float quadratic);

    const glm::vec3 &GetAmbient() const;
    void SetAmbient(const glm::vec3 &ambient);

    const glm::vec3 &GetDiffuse() const;
    void SetDiffuse(const glm::vec3 &diffuse);

    const glm::vec3 &GetSpecular() const;
    void SetSpecular(const glm::vec3 &specular);

    const glm::vec3 &GetColor() const;
    void SetColor(const glm::vec3 &color);
};


#endif //OPENGLGP_SPOTLIGHT_H
