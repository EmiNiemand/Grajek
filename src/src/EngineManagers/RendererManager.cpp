#include "EngineManagers/RendererManager.h"
#include "LowLevelClasses/Shader.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Lights/PointLight.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "Components/Renderers/Lights/SpotLight.h"

RendererManager::RendererManager(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {
    shader = std::make_shared<Shader>("basic.vert", "basic.frag");
    cubeMapShader = std::make_shared<Shader>("cubeMap.vert", "cubeMap.frag");
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)gloomEngine->width/(float)gloomEngine->height,
                                  0.1f, 100.0f);
}

RendererManager::~RendererManager() {}

void RendererManager::Free() {
    shader->Delete();
    cubeMapShader->Delete();
}

void RendererManager::UpdateRenderer() {
    UpdateProjection();
    UpdateCamera();
}

void RendererManager::UpdateProjection() {
    shader->Activate();
    shader->SetMat4("projection", projection);

    cubeMapShader->Activate();
    cubeMapShader->SetMat4("projection", projection);
}

void RendererManager::UpdateCamera() {
    shader->Activate();
    shader->SetMat4("view", Camera::activeCamera->GetComponent<Camera>()->GetViewMatrix());
    shader->SetVec3("viewPos", Camera::activeCamera->transform->GetGlobalPosition());

    cubeMapShader->Activate();
    cubeMapShader->SetMat4("view", Camera::activeCamera->GetComponent<Camera>()->GetViewMatrix());
    cubeMapShader->SetVec3("viewPos", Camera::activeCamera->transform->GetGlobalPosition());
}

void RendererManager::UpdateLight(int componentId) {
    for (int i = 0; i < spotLights.size(); i++) {
        if (spotLights.at(i) != nullptr && spotLights.at(i)->GetId() == componentId) {
            UpdateSpotLight(i);
            return;
        }
    }
    for (int i = 0; i < directionalLights.size(); i++) {
        if (directionalLights.at(i) != nullptr && directionalLights.at(i)->GetId() == componentId) {
            UpdateDirectionalLight(i);
            return;
        }
    }
    for (int i = 0; i < pointLights.size(); i++) {
        if (pointLights.at(i) != nullptr && pointLights.at(i)->GetId() == componentId) {
            UpdatePointLight(i);
            return;
        }
    }
}

void RendererManager::RemoveLight(int componentId) {
    for (int i = 0; i < spotLights.size(); i++) {
        if (spotLights.at(i) != nullptr && spotLights.at(i)->GetId() == componentId) {
            RemoveSpotLight(i);
            return;
        }
    }
    for (int i = 0; i < directionalLights.size(); i++) {
        if (directionalLights.at(i) != nullptr && directionalLights.at(i)->GetId() == componentId) {
            RemoveDirectionalLight(i);
            return;
        }
    }
    for (int i = 0; i < pointLights.size(); i++) {
        if (pointLights.at(i) != nullptr && pointLights.at(i)->GetId() == componentId) {
            RemovePointLight(i);
            return;
        }
    }
}

void RendererManager::UpdatePointLight(int lightNumber) {
    shader->Activate();
    std::shared_ptr<PointLight> pointLight = pointLights.find(lightNumber)->second;
    std::string light = "pointLights[" + std::to_string(lightNumber) + "]";
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

void RendererManager::UpdateDirectionalLight(int lightNumber) {
    shader->Activate();
    std::shared_ptr<DirectionalLight> directionalLight = directionalLights.find(lightNumber)->second;
    std::string light = "directionalLights[" + std::to_string(lightNumber) + "]";
    shader->SetBool(light + ".isActive", directionalLight->enabled);
    shader->SetVec3(light + ".direction", directionalLight->GetParent()->transform->GetForward());
    shader->SetVec3(light + ".ambient", directionalLight->GetAmbient());
    shader->SetVec3(light + ".diffuse", directionalLight->GetDiffuse());
    shader->SetVec3(light + ".specular", directionalLight->GetSpecular());
    shader->SetVec3(light + ".color", directionalLight->GetColor());
}

void RendererManager::UpdateSpotLight(int lightNumber) {
    shader->Activate();
    std::shared_ptr<SpotLight> spotLight = spotLights.find(lightNumber)->second;
    std::string light = "spotLights[" + std::to_string(lightNumber) + "]";
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

void RendererManager::RemovePointLight(int lightNumber) {
    shader->Activate();
    std::string light = "pointLights[" + std::to_string(lightNumber) + "]";
    shader->SetBool(light + ".isActive", false);
    shader->SetVec3(light + ".position", {0, 0, 0});
    shader->SetFloat(light + ".constant", 0);
    shader->SetFloat(light + ".linear", 0);
    shader->SetFloat(light + ".quadratic", 0);
    shader->SetVec3(light + ".ambient", {0, 0, 0});
    shader->SetVec3(light + ".diffuse", {0, 0, 0});
    shader->SetVec3(light + ".specular", {0, 0, 0});
    shader->SetVec3(light + ".color", {0, 0, 0});

    pointLights.find(lightNumber)->second = nullptr;
}

void RendererManager::RemoveDirectionalLight(int lightNumber) {
    shader->Activate();
    std::string light = "directionalLights[" + std::to_string(lightNumber) + "]";
    shader->SetBool(light +".isActive", false);
    shader->SetVec3(light + ".direction", {0, 0, 0});
    shader->SetVec3(light + ".ambient", {0, 0, 0});
    shader->SetVec3(light + ".diffuse", {0, 0, 0});
    shader->SetVec3(light + ".specular", {0, 0, 0});
    shader->SetVec3(light + ".color", {0, 0, 0});

    directionalLights.find(lightNumber)->second = nullptr;
}

void RendererManager::RemoveSpotLight(int lightNumber) {
    shader->Activate();
    std::shared_ptr<SpotLight> spotLight = spotLights.find(lightNumber)->second;
    std::string light = "spotLights[" + std::to_string(lightNumber) + "]";
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

    spotLights.find(lightNumber)->second = nullptr;
}

void RendererManager::SetFov(float fov) {
    RendererManager::fov = fov;
    UpdateProjection();
}
