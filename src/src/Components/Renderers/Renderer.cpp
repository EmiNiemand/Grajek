//
// Created by szymo on 02/03/2023.
//

#include "include/Components/Renderers/Renderer.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/GameObjectsAndPrefabs/GameObject.h"
#include "include/LowLevelClasses/Model.h"
#include "include/Components/Renderers/Camera.h"

Renderer::Renderer(const std::shared_ptr <GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id) :
Component(gloomEngine, parent, id) {
    objectColor = {1.0f, 1.0f, 1.0f};
    shininess = 32.0f;
}

Renderer::~Renderer() {

}

void Renderer::Update() {
    if(path.empty()) return;
    Draw();
    Component::Update();
}

void Renderer::LoadModel(std::string newPath) {
    path = newPath;
    model = std::make_shared<Model>(path, gloomEngine->engineRenderer->shader, GL_TRIANGLES);
}

void Renderer::Draw() {
    gloomEngine->engineRenderer->shader->Activate();
    gloomEngine->engineRenderer->shader->SetMat4("model", parent->transform->GetModelMatrix());
    gloomEngine->engineRenderer->shader->SetFloat("shininess", shininess);
    gloomEngine->engineRenderer->shader->SetVec3("objectColor", objectColor);
    model->Draw();
}

