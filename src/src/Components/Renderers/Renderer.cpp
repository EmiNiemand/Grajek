//
// Created by szymo on 02/03/2023.
//

#include <utility>

#include "include/Components/Renderers/Renderer.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/GameObjectsAndPrefabs/GameObject.h"
#include "include/LowLevelClasses/Model.h"
#include "include/Components/Renderers/Camera.h"

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
    model = std::make_shared<Model>( "res/models/"+path, gloomEngine->engineRenderer->shader, GL_TRIANGLES);
}

void Renderer::Draw() {
    if(path.empty()) return;

    gloomEngine->engineRenderer->shader->Activate();
    gloomEngine->engineRenderer->shader->SetMat4("model", parent->transform->GetModelMatrix());
    gloomEngine->engineRenderer->shader->SetFloat("shininess", shininess);
    gloomEngine->engineRenderer->shader->SetVec3("objectColor", objectColor);
    model->Draw();
}

