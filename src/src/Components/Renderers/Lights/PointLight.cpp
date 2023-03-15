#include "include/Components/Renderers/Lights/PointLight.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"

PointLight::PointLight(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id)
        : Component(gloomEngine, parent, id) {
    name = ComponentNames::POINTLIGHT;
    constant = 1.0f;
    linear = 0.7f;
    quadratic = 2.5f;
    ambient = {1.0f, 1.0f, 1.0f};
    diffuse = {1.0f, 1.0f, 1.0f};
    specular = {1.0f, 1.0f, 1.0f};
    color = {1.0f, 1.0f, 1.0f};
}

PointLight::~PointLight() {}

float PointLight::GetConstant() const {
    return constant;
}

void PointLight::SetConstant(float constant) {
    PointLight::constant = constant;
    Component::OnUpdate();
}

float PointLight::GetLinear() const {
    return linear;
}

void PointLight::SetLinear(float linear) {
    PointLight::linear = linear;
    Component::OnUpdate();
}

float PointLight::GetQuadratic() const {
    return quadratic;
}

void PointLight::SetQuadratic(float quadratic) {
    PointLight::quadratic = quadratic;
    Component::OnUpdate();
}

const glm::vec3 &PointLight::GetAmbient() const {
    return ambient;
}

void PointLight::SetAmbient(const glm::vec3 &ambient) {
    PointLight::ambient = ambient;
    Component::OnUpdate();
}

const glm::vec3 &PointLight::GetDiffuse() const {
    return diffuse;
}

void PointLight::SetDiffuse(const glm::vec3 &diffuse) {
    PointLight::diffuse = diffuse;
    Component::OnUpdate();
}

const glm::vec3 &PointLight::GetSpecular() const {
    return specular;
}

void PointLight::SetSpecular(const glm::vec3 &specular) {
    PointLight::specular = specular;
    Component::OnUpdate();
}

const glm::vec3 &PointLight::GetColor() const {
    return color;
}

void PointLight::SetColor(const glm::vec3 &color) {
    PointLight::color = color;
    Component::OnUpdate();
}
