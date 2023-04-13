#include "EngineManagers/RendererManager.h"
#include "GloomEngine.h"
#include "LowLevelClasses/Shader.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Drawable.h"
#include "Components/Renderers/Lights/PointLight.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "Components/Renderers/Lights/SpotLight.h"
#include "stb_image.h"

RendererManager::RendererManager() {
    shader = std::make_shared<Shader>("basic.vert", "basic.frag");
    cubeMapShader = std::make_shared<Shader>("cubeMap.vert", "cubeMap.frag");
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)GloomEngine::GetInstance()->width/(float)GloomEngine::GetInstance()->height,
                                  0.1f, 100.0f);
}

RendererManager::~RendererManager() = default;

RendererManager* RendererManager::GetInstance() {
    if (rendererManager == nullptr) {
        rendererManager = new RendererManager();
    }
    return rendererManager;
}

void RendererManager::Free() const {
    shader->Delete();
    cubeMapShader->Delete();
}

void RendererManager::DrawObjects() {
    for (const auto& drawable : drawBuffer) {
        drawable->Draw();
    }
    drawBuffer.clear();
}

void RendererManager::AddToDrawBuffer(const std::shared_ptr<Drawable>& DrawableComponent) {
    drawBuffer.emplace_back(DrawableComponent);
}

void RendererManager::UpdateProjection() const {
    shader->Activate();
    shader->SetMat4("projection", projection);

    cubeMapShader->Activate();
    cubeMapShader->SetMat4("projection", projection);

}

void RendererManager::UpdateCamera() const {
    shader->Activate();
    shader->SetMat4("view", Camera::activeCamera->GetComponent<Camera>()->GetViewMatrix());
    shader->SetVec3("viewPos", Camera::activeCamera->transform->GetGlobalPosition());

    cubeMapShader->Activate();
    cubeMapShader->SetMat4("view", glm::mat4(glm::mat3(Camera::activeCamera->GetComponent<Camera>()->GetViewMatrix())));
}

void RendererManager::UpdateLight(int componentId) {

    for (int i = 0; i < spotLights.size(); i++) {
        if (spotLights.at(i) != nullptr && spotLights.at(i)->GetId() == componentId) {
            UpdateSpotLight(i, shader);
            return;
        }
    }
    for (int i = 0; i < directionalLights.size(); i++) {
        if (directionalLights.at(i) != nullptr && directionalLights.at(i)->GetId() == componentId) {
            UpdateDirectionalLight(i, shader);
            return;
        }
    }
    for (int i = 0; i < pointLights.size(); i++) {
        if (pointLights.at(i) != nullptr && pointLights.at(i)->GetId() == componentId) {
            UpdatePointLight(i, shader);
            return;
        }
    }
}

void RendererManager::RemoveLight(int componentId) {
    for (int i = 0; i < spotLights.size(); i++) {
        if (spotLights.at(i) != nullptr && spotLights.at(i)->GetId() == componentId) {
            RemoveSpotLight(i, shader);
            return;
        }
    }
    for (int i = 0; i < directionalLights.size(); i++) {
        if (directionalLights.at(i) != nullptr && directionalLights.at(i)->GetId() == componentId) {
            RemoveDirectionalLight(i, shader);
            return;
        }
    }
    for (int i = 0; i < pointLights.size(); i++) {
        if (pointLights.at(i) != nullptr && pointLights.at(i)->GetId() == componentId) {
            RemovePointLight(i, shader);
            return;
        }
    }
}

void RendererManager::UpdatePointLight(int lightNumber, const std::shared_ptr<Shader>& lightShader) {
    lightShader->Activate();
    std::shared_ptr<PointLight> pointLight = pointLights.find(lightNumber)->second;
    std::string light = "pointLights[" + std::to_string(lightNumber) + "]";
    lightShader->SetBool(light + ".isActive", pointLight->enabled);
    lightShader->SetVec3(light + ".position", pointLight->GetParent()->transform->GetLocalPosition());
    lightShader->SetFloat(light + ".constant", pointLight->GetConstant());
    lightShader->SetFloat(light + ".linear", pointLight->GetLinear());
    lightShader->SetFloat(light + ".quadratic", pointLight->GetQuadratic());
    lightShader->SetVec3(light + ".ambient", pointLight->GetAmbient());
    lightShader->SetVec3(light + ".diffuse", pointLight->GetDiffuse());
    lightShader->SetVec3(light + ".specular", pointLight->GetSpecular());
    lightShader->SetVec3(light + ".color", pointLight->GetColor());
}

void RendererManager::UpdateDirectionalLight(int lightNumber, const std::shared_ptr<Shader>& lightShader) {
    lightShader->Activate();
    std::shared_ptr<DirectionalLight> directionalLight = directionalLights.find(lightNumber)->second;
    std::string light = "directionalLights[" + std::to_string(lightNumber) + "]";
    lightShader->SetBool(light + ".isActive", directionalLight->enabled);
    lightShader->SetVec3(light + ".direction", directionalLight->GetParent()->transform->GetForward());
    lightShader->SetVec3(light + ".ambient", directionalLight->GetAmbient());
    lightShader->SetVec3(light + ".diffuse", directionalLight->GetDiffuse());
    lightShader->SetVec3(light + ".specular", directionalLight->GetSpecular());
    lightShader->SetVec3(light + ".color", directionalLight->GetColor());
}

void RendererManager::UpdateSpotLight(int lightNumber, const std::shared_ptr<Shader>& lightShader) {
    lightShader->Activate();
    std::shared_ptr<SpotLight> spotLight = spotLights.find(lightNumber)->second;
    std::string light = "spotLights[" + std::to_string(lightNumber) + "]";
    lightShader->SetBool(light + ".isActive", spotLight->enabled);
    lightShader->SetVec3(light + ".position", spotLight->GetParent()->transform->GetLocalPosition());
    lightShader->SetVec3(light + ".direction", spotLight->GetParent()->transform->GetForward());
    lightShader->SetFloat(light + ".cutOff", spotLight->GetCutOff());
    lightShader->SetFloat(light + ".outerCutOff", spotLight->GetOuterCutOff());
    lightShader->SetFloat(light + ".constant", spotLight->GetConstant());
    lightShader->SetFloat(light + ".linear", spotLight->GetLinear());
    lightShader->SetFloat(light + ".quadratic", spotLight->GetQuadratic());
    lightShader->SetVec3(light + ".ambient", spotLight->GetAmbient());
    lightShader->SetVec3(light + ".diffuse", spotLight->GetDiffuse());
    lightShader->SetVec3(light + ".specular", spotLight->GetSpecular());
    lightShader->SetVec3(light + ".color", spotLight->GetColor());
}

void RendererManager::RemovePointLight(int lightNumber, const std::shared_ptr<Shader>& lightShader) {
    lightShader->Activate();
    std::string light = "pointLights[" + std::to_string(lightNumber) + "]";
    lightShader->SetBool(light + ".isActive", false);
    lightShader->SetVec3(light + ".position", {0, 0, 0});
    lightShader->SetFloat(light + ".constant", 0);
    lightShader->SetFloat(light + ".linear", 0);
    lightShader->SetFloat(light + ".quadratic", 0);
    lightShader->SetVec3(light + ".ambient", {0, 0, 0});
    lightShader->SetVec3(light + ".diffuse", {0, 0, 0});
    lightShader->SetVec3(light + ".specular", {0, 0, 0});
    lightShader->SetVec3(light + ".color", {0, 0, 0});

    pointLights.find(lightNumber)->second.reset();
}

void RendererManager::RemoveDirectionalLight(int lightNumber, const std::shared_ptr<Shader>& lightShader) {
    lightShader->Activate();
    std::string light = "directionalLights[" + std::to_string(lightNumber) + "]";
    lightShader->SetBool(light +".isActive", false);
    lightShader->SetVec3(light + ".direction", {0, 0, 0});
    lightShader->SetVec3(light + ".ambient", {0, 0, 0});
    lightShader->SetVec3(light + ".diffuse", {0, 0, 0});
    lightShader->SetVec3(light + ".specular", {0, 0, 0});
    lightShader->SetVec3(light + ".color", {0, 0, 0});

    directionalLights.find(lightNumber)->second.reset();
}

void RendererManager::RemoveSpotLight(int lightNumber, const std::shared_ptr<Shader>& lightShader) {
    lightShader->Activate();
    std::shared_ptr<SpotLight> spotLight = spotLights.find(lightNumber)->second;
    std::string light = "spotLights[" + std::to_string(lightNumber) + "]";
    lightShader->SetBool(light +".isActive", false);
    lightShader->SetVec3(light + ".position", {0, 0, 0});
    lightShader->SetVec3(light + ".direction", {0, 0, 0});
    lightShader->SetFloat(light + ".cutOff", 0);
    lightShader->SetFloat(light + ".outerCutOff", 0);
    lightShader->SetFloat(light + ".constant", 0);
    lightShader->SetFloat(light + ".linear", 0);
    lightShader->SetFloat(light + ".quadratic", 0);
    lightShader->SetVec3(light + ".ambient", {0, 0, 0});
    lightShader->SetVec3(light + ".diffuse", {0, 0, 0});
    lightShader->SetVec3(light + ".specular", {0, 0, 0});
    lightShader->SetVec3(light + ".color", {0, 0, 0});

    spotLights.find(lightNumber)->second.reset();
}

void RendererManager::SetFov(float fov) {
    RendererManager::fov = fov;
    UpdateProjection();
}
