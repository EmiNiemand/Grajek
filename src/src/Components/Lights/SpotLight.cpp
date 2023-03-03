//
// Created by szymo on 03/03/2023.
//

#include "include/Components/Lights/SpotLight.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"

SpotLight::SpotLight(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id)
        : Component(gloomEngine, parent), id(id) {
    name = ComponentNames::SPOTLIGHT;
    cutOff = glm::cos(glm::radians(12.5f));
    outerCutOff = glm::cos(glm::radians(15.0f));
    constant = 1.0f;
    linear = 0.045f;
    quadratic = 0.0075f;
    ambient = {0.0f, 0.0f, 0.0f};
    diffuse = {1.0f, 1.0f, 1.0f};
    specular = {1.0f, 1.0f, 1.0f};
    color = {1.0f, 1.0f, 1.0f};
}

SpotLight::~SpotLight() {}

void SpotLight::OnTransformUpdate() {
    gloomEngine->engineRenderer->UpdateSpotLight(id);
    Component::OnTransformUpdate();
}

void SpotLight::OnRemove() {
    gloomEngine->engineRenderer->RemoveSpotLight(id);
    Component::OnRemove();
}

float SpotLight::GetCutOff() const {
    return cutOff;
}

void SpotLight::SetCutOff(float cutOff) {
    SpotLight::cutOff = cutOff;
    gloomEngine->engineRenderer->UpdateSpotLight(id);
}

float SpotLight::GetOuterCutOff() const {
    return outerCutOff;
}

void SpotLight::SetOuterCutOff(float outerCutOff) {
    SpotLight::outerCutOff = outerCutOff;
    gloomEngine->engineRenderer->UpdateSpotLight(id);
}

float SpotLight::GetConstant() const {
    return constant;
}

void SpotLight::SetConstant(float constant) {
    SpotLight::constant = constant;
    gloomEngine->engineRenderer->UpdateSpotLight(id);
}

float SpotLight::GetLinear() const {
    return linear;
}

void SpotLight::SetLinear(float linear) {
    SpotLight::linear = linear;
    gloomEngine->engineRenderer->UpdateSpotLight(id);
}

float SpotLight::GetQuadratic() const {
    return quadratic;
}

void SpotLight::SetQuadratic(float quadratic) {
    SpotLight::quadratic = quadratic;
    gloomEngine->engineRenderer->UpdateSpotLight(id);
}

const glm::vec3 &SpotLight::GetAmbient() const {
    return ambient;
}

void SpotLight::SetAmbient(const glm::vec3 &ambient) {
    SpotLight::ambient = ambient;
    gloomEngine->engineRenderer->UpdateSpotLight(id);
}

const glm::vec3 &SpotLight::GetDiffuse() const {
    return diffuse;
}

void SpotLight::SetDiffuse(const glm::vec3 &diffuse) {
    SpotLight::diffuse = diffuse;
    gloomEngine->engineRenderer->UpdateSpotLight(id);
}

const glm::vec3 &SpotLight::GetSpecular() const {
    return specular;
}

void SpotLight::SetSpecular(const glm::vec3 &specular) {
    SpotLight::specular = specular;
    gloomEngine->engineRenderer->UpdateSpotLight(id);
}

const glm::vec3 &SpotLight::GetColor() const {
    return color;
}

void SpotLight::SetColor(const glm::vec3 &color) {
    SpotLight::color = color;
    gloomEngine->engineRenderer->UpdateSpotLight(id);
}

