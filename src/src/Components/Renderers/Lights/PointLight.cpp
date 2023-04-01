#include "Components/Renderers/Lights/PointLight.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"

PointLight::PointLight(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {
    constant = 1.0f;
    linear = 0.007f;
    quadratic = 0.0002f;
    ambient = {1.0f, 1.0f, 1.0f};
    diffuse = {0.69f, 0.69f, 0.69f};
    specular = {1.0f, 1.0f, 1.0f};
    color = {1.0f, 1.0f, 1.0f};
}

PointLight::~PointLight() = default;

void PointLight::OnCreate() {
    bool isAdded = false;
    int number = 0;

    for (auto&& pointLight : RendererManager::GetInstance()->pointLights) {
        if (pointLight.second == nullptr) {
            RendererManager::GetInstance()->pointLights.find(number)->second =
                    std::dynamic_pointer_cast<PointLight>(shared_from_this());
            isAdded = true;
        }
        number++;
    }
    if (!isAdded) RendererManager::GetInstance()->pointLights.insert(
            {number, std::dynamic_pointer_cast<PointLight>(shared_from_this())});
    RendererManager::GetInstance()->UpdateLight(id);
    Component::OnCreate();
}

void PointLight::OnDestroy() {
    RendererManager::GetInstance()->RemoveLight(id);
    Component::OnDestroy();
}

float PointLight::GetConstant() const {
    return constant;
}

void PointLight::SetConstant(float constant) {
    PointLight::constant = constant;
    OnUpdate();
    Component::OnUpdate();
}

float PointLight::GetLinear() const {
    return linear;
}

void PointLight::SetLinear(float linear) {
    PointLight::linear = linear;
    OnUpdate();
    Component::OnUpdate();
}

float PointLight::GetQuadratic() const {
    return quadratic;
}

void PointLight::SetQuadratic(float quadratic) {
    PointLight::quadratic = quadratic;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &PointLight::GetAmbient() const {
    return ambient;
}

void PointLight::SetAmbient(const glm::vec3 &ambient) {
    PointLight::ambient = ambient;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &PointLight::GetDiffuse() const {
    return diffuse;
}

void PointLight::SetDiffuse(const glm::vec3 &diffuse) {
    PointLight::diffuse = diffuse;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &PointLight::GetSpecular() const {
    return specular;
}

void PointLight::SetSpecular(const glm::vec3 &specular) {
    PointLight::specular = specular;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &PointLight::GetColor() const {
    return color;
}

void PointLight::SetColor(const glm::vec3 &color) {
    PointLight::color = color;
    OnUpdate();
    Component::OnUpdate();
}

void PointLight::OnUpdate() {
    RendererManager::GetInstance()->UpdateLight(id);
    Component::OnUpdate();
}
