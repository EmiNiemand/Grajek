#include "Components/Renderers/Lights/SpotLight.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"

SpotLight::SpotLight(const std::shared_ptr<GameObject> &parent, uint32_t id) : Component(parent, id) {
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

SpotLight::~SpotLight() = default;

void SpotLight::OnCreate() {
    bool isAdded = false;
    int number = 0;

    for (auto&& spotLight : RendererManager::GetInstance()->spotLights) {
        if (spotLight.second == nullptr) {
            RendererManager::GetInstance()->spotLights.find(number)->second =
                    std::dynamic_pointer_cast<SpotLight>(shared_from_this());
            isAdded = true;
        }
        number++;
    }
    if (!isAdded) RendererManager::GetInstance()->spotLights.insert(
            {number, std::dynamic_pointer_cast<SpotLight>(shared_from_this())});
    RendererManager::GetInstance()->UpdateLight(id);
    Component::OnCreate();
}

void SpotLight::OnDestroy() {
    RendererManager::GetInstance()->RemoveLight(id);
    Component::OnDestroy();
}

float SpotLight::GetCutOff() const {
    return cutOff;
}

void SpotLight::SetCutOff(float cutOff) {
    SpotLight::cutOff = cutOff;
    OnUpdate();
    Component::OnUpdate();
}

float SpotLight::GetOuterCutOff() const {
    return outerCutOff;
}

void SpotLight::SetOuterCutOff(float outerCutOff) {
    SpotLight::outerCutOff = outerCutOff;
    OnUpdate();
    Component::OnUpdate();
}

float SpotLight::GetConstant() const {
    return constant;
}

void SpotLight::SetConstant(float constant) {
    SpotLight::constant = constant;
    OnUpdate();
    Component::OnUpdate();
}

float SpotLight::GetLinear() const {
    return linear;
}

void SpotLight::SetLinear(float linear) {
    SpotLight::linear = linear;
    OnUpdate();
    Component::OnUpdate();
}

float SpotLight::GetQuadratic() const {
    return quadratic;
}

void SpotLight::SetQuadratic(float quadratic) {
    SpotLight::quadratic = quadratic;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &SpotLight::GetAmbient() const {
    return ambient;
}

void SpotLight::SetAmbient(const glm::vec3 &ambient) {
    SpotLight::ambient = ambient;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &SpotLight::GetDiffuse() const {
    return diffuse;
}

void SpotLight::SetDiffuse(const glm::vec3 &diffuse) {
    SpotLight::diffuse = diffuse;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &SpotLight::GetSpecular() const {
    return specular;
}

void SpotLight::SetSpecular(const glm::vec3 &specular) {
    SpotLight::specular = specular;
    OnUpdate();
    Component::OnUpdate();
}

const glm::vec3 &SpotLight::GetColor() const {
    return color;
}

void SpotLight::SetColor(const glm::vec3 &color) {
    SpotLight::color = color;
    OnUpdate();
    Component::OnUpdate();
}

void SpotLight::OnUpdate() {
    RendererManager::GetInstance()->UpdateLight(id);
    Component::OnUpdate();
}

