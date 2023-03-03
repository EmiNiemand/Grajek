//
// Created by szymo on 03/03/2023.
//

#include "include/Components/Lights/PointLight.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"

PointLight::PointLight(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id)
        : Component(gloomEngine, parent), id(id) {
    name = ComponentNames::POINTLIGHT;
    constant = 1.0f;
    linear = 0.00014f;
    quadratic = 0.0000007f;
    ambient = {0.05f, 0.05f, 0.05f};
    diffuse = {0.8f, 0.8f, 0.8f};
    specular = {1.0f, 1.0f, 1.0f};
    color = {1.0f, 1.0f, 1.0f};
}

PointLight::~PointLight() {}

void PointLight::OnTransformUpdate() {
    gloomEngine->engineRenderer->UpdatePointLight(id);
    Component::OnTransformUpdate();
}

void PointLight::OnRemove() {
    gloomEngine->engineRenderer->RemovePointLight(id);
    Component::OnRemove();
}

float PointLight::GetConstant() const {
    return constant;
}

void PointLight::SetConstant(float constant) {
    PointLight::constant = constant;
    gloomEngine->engineRenderer->UpdatePointLight(id);
}

float PointLight::GetLinear() const {
    return linear;
}

void PointLight::SetLinear(float linear) {
    PointLight::linear = linear;
    gloomEngine->engineRenderer->UpdatePointLight(id);
}

float PointLight::GetQuadratic() const {
    return quadratic;
}

void PointLight::SetQuadratic(float quadratic) {
    PointLight::quadratic = quadratic;
    gloomEngine->engineRenderer->UpdatePointLight(id);
}

const glm::vec3 &PointLight::GetAmbient() const {
    return ambient;
}

void PointLight::SetAmbient(const glm::vec3 &ambient) {
    PointLight::ambient = ambient;
    gloomEngine->engineRenderer->UpdatePointLight(id);
}

const glm::vec3 &PointLight::GetDiffuse() const {
    return diffuse;
}

void PointLight::SetDiffuse(const glm::vec3 &diffuse) {
    PointLight::diffuse = diffuse;
}

const glm::vec3 &PointLight::GetSpecular() const {
    return specular;
}

void PointLight::SetSpecular(const glm::vec3 &specular) {
    PointLight::specular = specular;
}

const glm::vec3 &PointLight::GetColor() const {
    return color;
}

void PointLight::SetColor(const glm::vec3 &color) {
    PointLight::color = color;
}
