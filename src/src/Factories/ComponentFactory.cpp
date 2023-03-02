//
// Created by szymo on 02/03/2023.
//

#include "include/Factiories/ComponentFactory.h"
#include "include/GloomEngine.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/Components/Renderer.h"
#include "include/Components/Camera.h"

ComponentFactory::ComponentFactory(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {}

std::shared_ptr<Renderer> ComponentFactory::CreateRenderer(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Renderer> component = std::make_shared<Renderer>(gloomEngine, parent);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    return component;
}

std::shared_ptr<Camera> ComponentFactory::CreateCamera(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Camera> component = std::make_shared<Camera>(gloomEngine, parent);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    return component;
}




