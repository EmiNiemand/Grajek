//
// Created by szymo on 03/03/2023.
//

#include "include/EngineComponents/EngineRenderer.h"
#include "include/LowLevelClasses/Shader.h"
#include "include/GloomEngine.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/Components/Camera.h"
#include "include/Components/Lights/PointLight.h"
#include "include/Components/Lights/DirectionalLight.h"
#include "include/Components/Lights/SpotLight.h"

EngineRenderer::EngineRenderer(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {
    shader = std::make_shared<Shader>("res/shaders/basic.vert", "res/shaders/basic.frag");
    projection = glm::perspective(glm::radians(45.0f), (float)*gloomEngine->width/(float)*gloomEngine->height, 0.1f, 100.0f);
}

EngineRenderer::~EngineRenderer() {}

void EngineRenderer::Destroy() {
    shader->Delete();
}

void EngineRenderer::UpdateRenderer() {
    UpdateProjection();
    UpdateCamera();
}

void EngineRenderer::UpdateProjection() {
    shader->Activate();
    shader->SetMat4("projection", projection);
}

void EngineRenderer::UpdateCamera() {
    shader->Activate();
    shader->SetMat4("view", std::dynamic_pointer_cast<Camera>(gloomEngine->activeCamera->FindComponent(ComponentNames::CAMERA))->GetViewMatrix());
    shader->SetVec3("viewPos", gloomEngine->activeCamera->transform->GetGlobalPosition());
}

void EngineRenderer::UpdatePointLight(int id) {
    shader->Activate();
    std::shared_ptr<PointLight> pointLight = pointLights.find(id)->second;
    std::string light = "pointLights[" + std::to_string(id) + "]";
    shader->SetBool(light + ".isActive", pointLight->enabled);
    shader->SetVec3(light + ".position", pointLight->GetParent()->transform->GetLocalPosition());
    shader->SetFloat(light + ".constant", pointLight->GetConstant());
    shader->SetFloat(light + ".linear", pointLight->GetLinear());
    shader->SetFloat(light + ".quadratic", pointLight->GetQuadratic());
    shader->SetVec3(light + ".ambient", pointLight->GetAmbient());
    shader->SetVec3(light + ".diffuse", pointLight->GetDiffuse());
    shader->SetVec3(light + ".specular", pointLight->GetSpecular());
    shader->SetVec3(light + ".color", pointLight->GetColor());
}

void EngineRenderer::UpdateDirectionalLight(int id) {
    shader->Activate();
    std::shared_ptr<DirectionalLight> directionalLight = directionalLights.find(id)->second;
    std::string light = "directionalLights[" + std::to_string(id) + "]";
    shader->SetBool(light + ".isActive", directionalLight->enabled);
    shader->SetVec3(light + ".direction", directionalLight->GetParent()->transform->GetForward());
    shader->SetVec3(light + ".ambient", directionalLight->GetAmbient());
    shader->SetVec3(light + ".diffuse", directionalLight->GetDiffuse());
    shader->SetVec3(light + ".specular", directionalLight->GetSpecular());
    shader->SetVec3(light + ".color", directionalLight->GetColor());
}

void EngineRenderer::UpdateSpotLight(int id) {
    shader->Activate();
    std::shared_ptr<SpotLight> spotLight = spotLights.find(id)->second;
    std::string light = "spotLights[" + std::to_string(id) + "]";
    shader->SetBool(light + ".isActive", spotLight->enabled);
    shader->SetVec3(light + ".position", spotLight->GetParent()->transform->GetLocalPosition());
    shader->SetVec3(light + ".direction", spotLight->GetParent()->transform->GetForward());
    shader->SetFloat(light + ".cutOff", spotLight->GetCutOff());
    shader->SetFloat(light + ".outerCutOff", spotLight->GetOuterCutOff());
    shader->SetFloat(light + ".constant", spotLight->GetConstant());
    shader->SetFloat(light + ".linear", spotLight->GetLinear());
    shader->SetFloat(light + ".quadratic", spotLight->GetQuadratic());
    shader->SetVec3(light + ".ambient", spotLight->GetAmbient());
    shader->SetVec3(light + ".diffuse", spotLight->GetDiffuse());
    shader->SetVec3(light + ".specular", spotLight->GetSpecular());
    shader->SetVec3(light + ".color", spotLight->GetColor());
}

void EngineRenderer::RemovePointLight(int id) {
    shader->Activate();
    std::string light = "pointLights[" + std::to_string(id) + "]";
    shader->SetBool(light + ".isActive", false);
    shader->SetVec3(light + ".position", {0, 0, 0});
    shader->SetFloat(light + ".constant", 0);
    shader->SetFloat(light + ".linear", 0);
    shader->SetFloat(light + ".quadratic", 0);
    shader->SetVec3(light + ".ambient", {0, 0, 0});
    shader->SetVec3(light + ".diffuse", {0, 0, 0});
    shader->SetVec3(light + ".specular", {0, 0, 0});
    shader->SetVec3(light + ".color", {0, 0, 0});

    pointLights.find(id)->second = nullptr;
}

void EngineRenderer::RemoveDirectionalLight(int id) {
    shader->Activate();
    std::string light = "directionalLights[" + std::to_string(id) + "]";
    shader->SetBool(light +".isActive", false);
    shader->SetVec3(light + ".direction", {0, 0, 0});
    shader->SetVec3(light + ".ambient", {0, 0, 0});
    shader->SetVec3(light + ".diffuse", {0, 0, 0});
    shader->SetVec3(light + ".specular", {0, 0, 0});
    shader->SetVec3(light + ".color", {0, 0, 0});

    directionalLights.find(id)->second = nullptr;
}

void EngineRenderer::RemoveSpotLight(int id) {
    shader->Activate();
    std::shared_ptr<SpotLight> spotLight = spotLights.find(id)->second;
    std::string light = "spotLights[" + std::to_string(id) + "]";
    shader->SetBool(light +".isActive", false);
    shader->SetVec3(light + ".position", {0, 0, 0});
    shader->SetVec3(light + ".direction", {0, 0, 0});
    shader->SetFloat(light + ".cutOff", 0);
    shader->SetFloat(light + ".outerCutOff", 0);
    shader->SetFloat(light + ".constant", 0);
    shader->SetFloat(light + ".linear", 0);
    shader->SetFloat(light + ".quadratic", 0);
    shader->SetVec3(light + ".ambient", {0, 0, 0});
    shader->SetVec3(light + ".diffuse", {0, 0, 0});
    shader->SetVec3(light + ".specular", {0, 0, 0});
    shader->SetVec3(light + ".color", {0, 0, 0});

    spotLights.find(id)->second = nullptr;
}

void EngineRenderer::SetFov(float fov) {
    EngineRenderer::fov = fov;
    UpdateProjection();
}
