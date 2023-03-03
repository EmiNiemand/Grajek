//
// Created by szymo on 02/03/2023.
//

#include "include/Factiories/ComponentFactory.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/Components/Renderer.h"
#include "include/Components/Camera.h"
#include "include/Components/Lights/PointLight.h"
#include "include/Components/Lights/DirectionalLight.h"
#include "include/Components/Lights/SpotLight.h"

ComponentFactory::ComponentFactory(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {}

std::shared_ptr<Renderer> ComponentFactory::CreateRenderer(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Renderer> component = std::dynamic_pointer_cast<Renderer>(parent->FindComponent(ComponentNames::RENDERER));
    if (component != nullptr) return component;
    component = std::make_shared<Renderer>(gloomEngine, parent);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    return component;
}

std::shared_ptr<Camera> ComponentFactory::CreateCamera(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Camera> component = std::dynamic_pointer_cast<Camera>(parent->FindComponent(ComponentNames::CAMERA));
    if (component != nullptr) return component;
    component = std::make_shared<Camera>(gloomEngine, parent);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    return component;
}

std::shared_ptr<PointLight> ComponentFactory::CreatePointLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<PointLight> component = std::dynamic_pointer_cast<PointLight>(parent->FindComponent(ComponentNames::POINTLIGHT));
    if (component != nullptr) return component;
    int id;
    for (auto&& pointLight : gloomEngine->engineRenderer->pointLights){
        if (pointLight.second == nullptr) {
            id = pointLight.first;
            component = std::make_shared<PointLight>(gloomEngine, parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            parent->AddComponent(parentComponent);
            gloomEngine->AddComponent(component);
            gloomEngine->engineRenderer->pointLights.find(id)->second = component;
            gloomEngine->engineRenderer->UpdatePointLight(id);
            return component;
        }
    }

    id = gloomEngine->engineRenderer->pointLightNumber;
    component = std::make_shared<PointLight>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    gloomEngine->engineRenderer->pointLights.insert({id, component});
    gloomEngine->engineRenderer->UpdatePointLight(id);
    gloomEngine->engineRenderer->pointLightNumber++;
    return component;

}

std::shared_ptr<DirectionalLight> ComponentFactory::CreateDirectionalLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<DirectionalLight> component = std::dynamic_pointer_cast<DirectionalLight>(parent->FindComponent(ComponentNames::DIRECTIONALLIGHT));
    if (component != nullptr) return component;
    int id;
    for (auto&& directionalLight : gloomEngine->engineRenderer->directionalLights){
        if (directionalLight.second == nullptr) {
            id = directionalLight.first;
            component = std::make_shared<DirectionalLight>(gloomEngine, parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            parent->AddComponent(parentComponent);
            gloomEngine->AddComponent(component);
            gloomEngine->engineRenderer->directionalLights.find(id)->second = component;
            gloomEngine->engineRenderer->UpdateDirectionalLight(id);
            return component;
        }
    }

    id = gloomEngine->engineRenderer->directionalLightNumber;
    component = std::make_shared<DirectionalLight>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    gloomEngine->engineRenderer->directionalLights.insert({id, component});
    gloomEngine->engineRenderer->UpdateDirectionalLight(id);
    gloomEngine->engineRenderer->directionalLightNumber++;
    return component;
}

std::shared_ptr<SpotLight> ComponentFactory::CreateSpotLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<SpotLight> component = std::dynamic_pointer_cast<SpotLight>(parent->FindComponent(ComponentNames::SPOTLIGHT));
    if (component != nullptr) return component;
    int id;
    for (auto&& spotLight : gloomEngine->engineRenderer->spotLights){
        if (spotLight.second == nullptr) {
            id = spotLight.first;
            component = std::make_shared<SpotLight>(gloomEngine, parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            parent->AddComponent(parentComponent);
            gloomEngine->AddComponent(component);
            gloomEngine->engineRenderer->spotLights.find(id)->second = component;
            gloomEngine->engineRenderer->UpdateSpotLight(id);
            return component;
        }
    }

    id = gloomEngine->engineRenderer->spotLightNumber;
    component = std::make_shared<SpotLight>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    gloomEngine->engineRenderer->spotLights.insert({id, component});
    gloomEngine->engineRenderer->UpdateSpotLight(id);
    gloomEngine->engineRenderer->spotLightNumber++;
    return component;
}




