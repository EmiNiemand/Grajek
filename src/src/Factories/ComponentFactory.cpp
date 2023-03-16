#include "include/Factories/ComponentFactory.h"
#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/EngineComponents/EngineColliders.h"
#include "include/GameObjectsAndPrefabs/GameObject.h"
#include "include/Components/Renderers/Renderer.h"
#include "include/Components/Renderers/Camera.h"
#include "include/Components/Renderers/Lights/PointLight.h"
#include "include/Components/Renderers/Lights/DirectionalLight.h"
#include "include/Components/Renderers/Lights/SpotLight.h"
#include "include/Components/PhysicsAndColliders/Rigidbody.h"
#include "include/Components/PhysicsAndColliders/BoxCollider.h"

ComponentFactory::ComponentFactory(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {}

std::shared_ptr<Component> ComponentFactory::CreateComponent(std::string type, const std::shared_ptr<GameObject> &parent){
    if (type == typeid(Renderer).name()) {
        std::shared_ptr<Component> component = CreateRenderer(parent);
        return component;
    }
    else if (type == typeid(Camera).name()) {
        std::shared_ptr<Component> component = CreateCamera(parent);
        return component;
    }
    else if (type == typeid(PointLight).name()) {
        std::shared_ptr<Component> component = CreatePointLight(parent);
        return component;
    }
    else if (type == typeid(DirectionalLight).name()) {
        std::shared_ptr<Component> component = CreateDirectionalLight(parent);
        return component;
    }
    else if (type == typeid(SpotLight).name()) {
        std::shared_ptr<Component> component = CreateSpotLight(parent);
        return component;
    }
    else if (type == typeid(BoxCollider).name()) {
        std::shared_ptr<Component> component = CreateBoxCollider(parent);
        return component;
    }
    else if (type == typeid(Rigidbody).name()) {
        std::shared_ptr<Component> component = CreateRigidbody(parent);
        return component;
    }
    return nullptr;
};

std::shared_ptr<Renderer> ComponentFactory::CreateRenderer(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Renderer> component = std::dynamic_pointer_cast<Renderer>(
            parent->GetComponentByName(ComponentNames::RENDERER));
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<Renderer>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    gloomEngine->AddComponent(component);
    return component;
}

std::shared_ptr<Camera> ComponentFactory::CreateCamera(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Camera> component = std::dynamic_pointer_cast<Camera>(
            parent->GetComponentByName(ComponentNames::CAMERA));
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<Camera>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    gloomEngine->AddComponent(component);
    return component;
}

std::shared_ptr<PointLight> ComponentFactory::CreatePointLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<PointLight> component = std::dynamic_pointer_cast<PointLight>(
            parent->GetComponentByName(ComponentNames::POINTLIGHT));
    if (component != nullptr) return component;
    id++;

    int number = 0;

    for (auto&& pointLight : gloomEngine->engineRenderer->pointLights) {
        if (pointLight.second == nullptr) {
            component = std::make_shared<PointLight>(gloomEngine, parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
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
    gloomEngine->AddComponent(component);
    gloomEngine->engineRenderer->pointLights.insert({number, component});
    gloomEngine->engineRenderer->UpdateLight(id);
    return component;

}

std::shared_ptr<DirectionalLight> ComponentFactory::CreateDirectionalLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<DirectionalLight> component = std::dynamic_pointer_cast<DirectionalLight>(
            parent->GetComponentByName(ComponentNames::DIRECTIONALLIGHT));
    if (component != nullptr) return component;
    id++;
    int number = 0;

    for (auto&& directionalLight : gloomEngine->engineRenderer->directionalLights){
        if (directionalLight.second == nullptr) {
            component = std::make_shared<DirectionalLight>(gloomEngine, parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
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
    gloomEngine->AddComponent(component);
    gloomEngine->engineRenderer->directionalLights.insert({number, component});
    gloomEngine->engineRenderer->UpdateLight(id);
    return component;
}

std::shared_ptr<SpotLight> ComponentFactory::CreateSpotLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<SpotLight> component = std::dynamic_pointer_cast<SpotLight>(
            parent->GetComponentByName(ComponentNames::SPOTLIGHT));
    if (component != nullptr) return component;
    id++;
    int number = 0;

    for (auto&& spotLight : gloomEngine->engineRenderer->spotLights){
        if (spotLight.second == nullptr) {
            component = std::make_shared<SpotLight>(gloomEngine, parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            gloomEngine->AddComponent(component);
            gloomEngine->engineRenderer->spotLights.find(number)->second = component;
            gloomEngine->engineRenderer->UpdateLight(id);
            return component;
        }
        number++;
    }
    component = std::make_shared<SpotLight>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    gloomEngine->AddComponent(component);
    gloomEngine->engineRenderer->spotLights.insert({number, component});
    gloomEngine->engineRenderer->UpdateLight(id);
    return component;
}

std::shared_ptr<BoxCollider> ComponentFactory::CreateBoxCollider(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<BoxCollider> component = std::dynamic_pointer_cast<BoxCollider>(
            parent->GetComponentByName(ComponentNames::BOXCOLLIDER));
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<BoxCollider>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    gloomEngine->engineColliders->boxColliders.insert({id, component});
    gloomEngine->engineColliders->OnBoxColliderAdd();
    gloomEngine->AddComponent(component);
    return component;
}

std::shared_ptr<Rigidbody> ComponentFactory::CreateRigidbody(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Rigidbody> component = std::dynamic_pointer_cast<Rigidbody>(
            parent->GetComponentByName(ComponentNames::RIGIDBODY));
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<Rigidbody>(gloomEngine, parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    gloomEngine->AddComponent(component);
    return component;
}
