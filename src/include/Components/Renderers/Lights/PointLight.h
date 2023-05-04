#ifndef OPENGLGP_POINTLIGHT_H
#define OPENGLGP_POINTLIGHT_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"

class PointLight : public Component {
private:
    float constant;
    float linear;
    float quadratic;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 color;

public:
    PointLight(const std::shared_ptr<GameObject> &parent, int id);
    ~PointLight() override;

    void OnCreate() override;
    void OnDestroy() override;

    void OnUpdate() override;

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


#endif //OPENGLGP_POINTLIGHT_H
