//
// Created by szymo on 02/03/2023.
//

#include "include/Factories/ComponentFactory.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/EngineComponents/EngineColliders.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/Components/Renderer.h"
#include "include/Components/Camera.h"
#include "include/Components/Lights/PointLight.h"
#include "include/Components/Lights/DirectionalLight.h"
#include "include/Components/Lights/SpotLight.h"
#include "include/Components/BoxCollider.h"

ComponentFactory::ComponentFactory(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {}

std::shared_ptr<Renderer> ComponentFactory::CreateRenderer(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Renderer> component = std::dynamic_pointer_cast<Renderer>(parent->FindComponentByName(ComponentNames::RENDERER));
    if (component != nullptr) return component;
    component = std::make_shared<Renderer>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    id++;
    return component;
}

std::shared_ptr<Camera> ComponentFactory::CreateCamera(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Camera> component = std::dynamic_pointer_cast<Camera>(parent->FindComponentByName(ComponentNames::CAMERA));
    if (component != nullptr) return component;
    component = std::make_shared<Camera>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    id++;
    return component;
}

std::shared_ptr<PointLight> ComponentFactory::CreatePointLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<PointLight> component = std::dynamic_pointer_cast<PointLight>(parent->FindComponentByName(ComponentNames::POINTLIGHT));
    if (component != nullptr) return component;

    int number = 0;

    for (auto&& pointLight : gloomEngine->engineRenderer->pointLights) {
        if (pointLight.second == nullptr) {
            component = std::make_shared<PointLight>(gloomEngine, parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            parent->AddComponent(parentComponent);
            gloomEngine->AddComponent(component);
            gloomEngine->engineRenderer->pointLights.find(number)->second = component;
            gloomEngine->engineRenderer->UpdateLight(id);
            id++;
            return component;
        }
        number++;
    }

    component = std::make_shared<PointLight>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    gloomEngine->engineRenderer->pointLights.insert({number, component});
    gloomEngine->engineRenderer->UpdateLight(id);
    id++;
    return component;

}

std::shared_ptr<DirectionalLight> ComponentFactory::CreateDirectionalLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<DirectionalLight> component = std::dynamic_pointer_cast<DirectionalLight>(parent->FindComponentByName(ComponentNames::DIRECTIONALLIGHT));
    if (component != nullptr) return component;

    int number = 0;

    for (auto&& directionalLight : gloomEngine->engineRenderer->directionalLights){
        if (directionalLight.second == nullptr) {
            component = std::make_shared<DirectionalLight>(gloomEngine, parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            parent->AddComponent(parentComponent);
            gloomEngine->AddComponent(component);
            gloomEngine->engineRenderer->directionalLights.find(number)->second = component;
            gloomEngine->engineRenderer->UpdateLight(id);
            id++;
            return component;
        }
        number++;
    }

    component = std::make_shared<DirectionalLight>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    gloomEngine->engineRenderer->directionalLights.insert({number, component});
    gloomEngine->engineRenderer->UpdateLight(id);
    id++;
    return component;
}

std::shared_ptr<SpotLight> ComponentFactory::CreateSpotLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<SpotLight> component = std::dynamic_pointer_cast<SpotLight>(parent->FindComponentByName(ComponentNames::SPOTLIGHT));
    if (component != nullptr) return component;

    int number = 0;

    for (auto&& spotLight : gloomEngine->engineRenderer->spotLights){
        if (spotLight.second == nullptr) {
            component = std::make_shared<SpotLight>(gloomEngine, parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            parent->AddComponent(parentComponent);
            gloomEngine->AddComponent(component);
            gloomEngine->engineRenderer->spotLights.find(number)->second = component;
            gloomEngine->engineRenderer->UpdateLight(id);
            return component;
        }
    }
    component = std::make_shared<SpotLight>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    gloomEngine->engineRenderer->spotLights.insert({number, component});
    gloomEngine->engineRenderer->UpdateLight(id);
    id++;
    return component;
}

std::shared_ptr<BoxCollider> ComponentFactory::CreateBoxCollider(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<BoxCollider> component = std::dynamic_pointer_cast<BoxCollider>(parent->FindComponentByName(ComponentNames::BOXCOLLIDER));
    if (component != nullptr) return component;
    component = std::make_shared<BoxCollider>(gloomEngine, parent, id);
    id++;
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    gloomEngine->engineColliders->boxColliders.insert({id, component});
    gloomEngine->engineColliders->OnBoxColliderAdd();
    parent->AddComponent(parentComponent);
    gloomEngine->AddComponent(component);
    return component;
}




