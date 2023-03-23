//
// Created by szymo on 02/03/2023.
//

#include <utility>

#include "Components/Renderers/Renderer.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/Model.h"
#include "Components/Renderers/Camera.h"

/**
 * @attention Remember to call LoadModel if you want model to actually display
 */
Renderer::Renderer(const std::shared_ptr <GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id) :
Component(gloomEngine, parent, id) {
    objectColor = {1.0f, 1.0f, 1.0f};
    shininess = 32.0f;
}

Renderer::~Renderer() {
    model.reset();
}

void Renderer::Update() {
    if(path.empty()) return;
    Draw();
    Component::Update();
}

/**
 * @attention Needs to be called after renderer's constructor
 * @param newPath - relative path starting in res/models/
 */
void Renderer::LoadModel(std::string newPath) {
    path = std::move(newPath);
    model = std::make_shared<Model>( "res/models/"+path, gloomEngine->rendererManager->shader, GL_TRIANGLES);
}

void Renderer::Draw() {
    if(path.empty()) return;

    gloomEngine->rendererManager->shader->Activate();
    gloomEngine->rendererManager->shader->SetMat4("model", parent->transform->GetModelMatrix());
    gloomEngine->rendererManager->shader->SetFloat("shininess", shininess);
    gloomEngine->rendererManager->shader->SetVec3("objectColor", objectColor);
    model->Draw();
}

void Renderer::OnUpdate() {
    gloomEngine->rendererManager->UpdateLight(id);
    Component::OnUpdate();
}

