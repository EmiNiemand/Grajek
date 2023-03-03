//
// Created by szymo on 03/03/2023.
//

#include "include/Components/Lights/DirectionalLight.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"

DirectionalLight::DirectionalLight(const std::shared_ptr<GloomEngine> &gloomEngine,
                                   const std::shared_ptr<GameObject> &parent, int id) : Component(gloomEngine, parent), id(id) {
    name = ComponentNames::DIRECTIONALLIGHT;
    ambient = {0.1f, 0.1f, 0.1f};
    diffuse = {0.25f, 0.25f, 0.25f};
    specular = {0.33f, 0.33f, 0.33f};
    color = {1.0f, 1.0f, 1.0f};
}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::OnTransformUpdate() {
    gloomEngine->engineRenderer->UpdateDirectionalLight(id);
    Component::OnTransformUpdate();
}

void DirectionalLight::OnRemove() {
    gloomEngine->engineRenderer->RemoveDirectionalLight(id);
    Component::OnRemove();
}

const glm::vec3 &DirectionalLight::GetAmbient() const {
    return ambient;
}

void DirectionalLight::SetAmbient(const glm::vec3 &ambient) {
    DirectionalLight::ambient = ambient;
    gloomEngine->engineRenderer->UpdateDirectionalLight(id);
}

const glm::vec3 &DirectionalLight::GetDiffuse() const {
    return diffuse;
}

void DirectionalLight::SetDiffuse(const glm::vec3 &diffuse) {
    DirectionalLight::diffuse = diffuse;
    gloomEngine->engineRenderer->UpdateDirectionalLight(id);
}

const glm::vec3 &DirectionalLight::GetSpecular() const {
    return specular;
}

void DirectionalLight::SetSpecular(const glm::vec3 &specular) {
    DirectionalLight::specular = specular;
    gloomEngine->engineRenderer->UpdateDirectionalLight(id);
}

const glm::vec3 &DirectionalLight::GetColor() const {
    return color;
}

void DirectionalLight::SetColor(const glm::vec3 &color) {
    DirectionalLight::color = color;
    gloomEngine->engineRenderer->UpdateDirectionalLight(id);
}
