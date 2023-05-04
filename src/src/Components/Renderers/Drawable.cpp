//
// Created by szymo on 01/04/2023.
//

#include "Components/Renderers/Drawable.h"
#include "EngineManagers/RendererManager.h"
#include "Other/FrustumCulling.h"
#include "GameObjectsAndPrefabs/GameObject.h"

Drawable::Drawable(const std::shared_ptr<GameObject> &parent, uint32_t id) : Component(parent, id) {}

Drawable::~Drawable() = default;

void Drawable::Update() {
    AddToDraw();
    Component::Update();
}

void Drawable::AddToDraw() {
    RendererManager::GetInstance()->AddToDrawBuffer(std::dynamic_pointer_cast<Drawable>(shared_from_this()));
}

