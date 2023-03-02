//
// Created by szymo on 02/03/2023.
//

#include "include/Components/Renderer.h"
#include "include/GloomEngine.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/Components/Camera.h"

Renderer::Renderer(const std::shared_ptr <GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent) :
Component(gloomEngine, parent) {
    name = ComponentNames::RENDERER;
}

Renderer::~Renderer() {
    shader.Delete();
}

void Renderer::Update() {
    if(path.empty()) return;
    Draw();
    Component::Update();
}

void Renderer::LoadModel(std::string newPath) {
    path = newPath;
    model = std::make_shared<Model>(path, &shader, GL_TRIANGLES);
}

void Renderer::Draw() {
    shader.Activate();
    shader.SetMat4("projection", gloomEngine->projection);
    shader.SetMat4("view", std::dynamic_pointer_cast<Camera>(gloomEngine->activeCamera->FindComponent(ComponentNames::CAMERA))->GetViewMatrix());
    shader.SetMat4("model", parent->transform.GetModelMatrix());
    model->Draw();
}

