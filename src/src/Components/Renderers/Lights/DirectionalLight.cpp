#include "Components/Renderers/Lights/DirectionalLight.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"

DirectionalLight::DirectionalLight(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {
    ambient = {0.1f, 0.1f, 0.1f};
    diffuse = {0.25f, 0.25f, 0.25f};
    specular = {0.33f, 0.33f, 0.33f};
    color = {1.0f, 1.0f, 1.0f};
}

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::OnCreate() {
    bool isAdded = false;
    int number = 0;

    for (auto&& directionalLight : RendererManager::GetInstance()->directionalLights) {
        if (directionalLight.second == nullptr) {
            RendererManager::GetInstance()->directionalLights.find(number)->second =
                    std::dynamic_pointer_cast<DirectionalLight>(shared_from_this());
            isAdded = true;
        }
        number++;
    }
    if (!isAdded) RendererManager::GetInstance()->directionalLights.insert(
            {number,std::dynamic_pointer_cast<DirectionalLight>(shared_from_this())});
    RendererManager::GetInstance()->UpdateLight(id);
    Component::OnCreate();
}

void DirectionalLight::OnDestroy() {
    RendererManager::GetInstance()->RemoveLight(id);
    Component::OnDestroy();
}

const glm::vec3 &DirectionalLight::GetAmbient() const {
    return ambient;
}

void DirectionalLight::SetAmbient(const glm::vec3 &ambient) {
    DirectionalLight::ambient = ambient;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &DirectionalLight::GetDiffuse() const {
    return diffuse;
}

void DirectionalLight::SetDiffuse(const glm::vec3 &diffuse) {
    DirectionalLight::diffuse = diffuse;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &DirectionalLight::GetSpecular() const {
    return specular;
}

void DirectionalLight::SetSpecular(const glm::vec3 &specular) {
    DirectionalLight::specular = specular;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &DirectionalLight::GetColor() const {
    return color;
}

void DirectionalLight::SetColor(const glm::vec3 &color) {
    DirectionalLight::color = color;
    OnUpdate();
    Component::OnUpdate();
}

void DirectionalLight::OnUpdate() {
    RendererManager::GetInstance()->UpdateLight(id);
    Component::OnUpdate();
}
