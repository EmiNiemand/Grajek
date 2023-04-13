#include "EngineManagers/ShadowManager.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Drawable.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "LowLevelClasses/Shader.h"

ShadowManager::ShadowManager() {
    shadowShader = std::make_shared<Shader>("shadowMap.vert", "shadowMap.frag");

    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    RendererManager::GetInstance()->shader->Activate();
    RendererManager::GetInstance()->shader->SetInt("shadowMap", depthMap);

    RendererManager::GetInstance()->animatedShader->Activate();
    RendererManager::GetInstance()->animatedShader->SetInt("shadowMap", depthMap);
}

ShadowManager::~ShadowManager() = default;

ShadowManager *ShadowManager::GetInstance() {
    if (shadowManager == nullptr) {
        shadowManager = new ShadowManager();
    }
    return shadowManager;
}

void ShadowManager::PrepareShadow() {
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    glm::vec3 lightPos = RendererManager::GetInstance()->directionalLights[0]->GetParent()->transform->GetGlobalPosition();
//    lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)shadowWidth / (GLfloat)shadowHeight, 0.1f, 10.0f); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
    lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, nearPlane, farPlane);
    lightView = glm::lookAt(lightPos, RendererManager::GetInstance()->directionalLights[0]->GetParent()->transform->GetForward(),
                            RendererManager::GetInstance()->directionalLights[0]->GetParent()->transform->GetUp());
    lightSpaceMatrix = lightProjection * lightView;

    // render scene from light's point of view
    shadowShader->Activate();
    shadowShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    RendererManager::GetInstance()->shader->Activate();
    RendererManager::GetInstance()->shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    RendererManager::GetInstance()->animatedShader->Activate();
    RendererManager::GetInstance()->animatedShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, shadowWidth, shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    for (const auto& drawable : RendererManager::GetInstance()->drawBuffer) {
        drawable->Draw(shadowShader);
    }
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}