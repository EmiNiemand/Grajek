//
// Created by szymo on 05/04/2023.
//

#include "Components/UI/UIComponent.h"
#include "EngineManagers/UIManager.h"

UIComponent::UIComponent(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

UIComponent::~UIComponent() = default;

void UIComponent::Update() {
    AddToDraw();
    Component::Update();
}

void UIComponent::AddToDraw() {
    UIManager::GetInstance()->AddToDrawBuffer(std::dynamic_pointer_cast<UIComponent>(shared_from_this()));
}