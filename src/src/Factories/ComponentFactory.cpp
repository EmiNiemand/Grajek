#include "Factories/ComponentFactory.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"
#include "EngineManagers/ColliderManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/CubeMap.h"
#include "Components/Renderers/Animator.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Lights/PointLight.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "Components/Renderers/Lights/SpotLight.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/PlayerMovement.h"

ComponentFactory* ComponentFactory::componentFactory = nullptr;

ComponentFactory::ComponentFactory() {}

ComponentFactory* ComponentFactory::GetInstance() {
    if (componentFactory == nullptr) {
        componentFactory = new ComponentFactory();
    }
    return componentFactory;
}

std::shared_ptr<Component> ComponentFactory::CreateComponent(std::string type, const std::shared_ptr<GameObject> &parent){
    if (type == typeid(Renderer).name()) {
        std::shared_ptr<Component> component = CreateRenderer(parent);
        return component;
    }
    else if (type == typeid(CubeMap).name()) {
        std::shared_ptr<Component> component = CreateCubeMap(parent);
        return component;
    }
    else if (type == typeid(Animator).name()) {
	    std::shared_ptr<Component> component = CreateAnimator(parent);
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
    else if (type == typeid(Image).name()) {
        std::shared_ptr<Component> component = CreateImage(parent);
        return component;
    }
    else if (type == typeid(PlayerMovement).name()) {
        std::shared_ptr<Component> component = CreatePlayerMovement(parent);
        return component;
    }
    return nullptr;
};

std::shared_ptr<Renderer> ComponentFactory::CreateRenderer(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Renderer> component = parent->GetComponent<Renderer>();
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<Renderer>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    GloomEngine::GetInstance()->AddComponent(component);
    return component;
}

std::shared_ptr<CubeMap> ComponentFactory::CreateCubeMap(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<CubeMap> component = parent->GetComponent<CubeMap>();
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<CubeMap>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    GloomEngine::GetInstance()->AddComponent(component);
    return component;
}

std::shared_ptr<Animator> ComponentFactory::CreateAnimator(const std::shared_ptr<GameObject> &parent) {
	std::shared_ptr<Animator> component = parent->GetComponent<Animator>();
	if (component != nullptr) return component;
	id++;
	component = std::make_shared<Animator>(parent, id);
	std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
	GloomEngine::GetInstance()->AddComponent(component);
	return component;
}

std::shared_ptr<Camera> ComponentFactory::CreateCamera(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Camera> component = parent->GetComponent<Camera>();
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<Camera>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    GloomEngine::GetInstance()->AddComponent(component);
    return component;
}

std::shared_ptr<PointLight> ComponentFactory::CreatePointLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<PointLight> component = parent->GetComponent<PointLight>();
    if (component != nullptr) return component;
    id++;

    int number = 0;

    for (auto&& pointLight : RendererManager::GetInstance()->pointLights) {
        if (pointLight.second == nullptr) {
            component = std::make_shared<PointLight>(parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            GloomEngine::GetInstance()->AddComponent(component);
            RendererManager::GetInstance()->pointLights.find(number)->second = component;
            RendererManager::GetInstance()->UpdateLight(id);
            id++;
            return component;
        }
        number++;
    }

    component = std::make_shared<PointLight>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    GloomEngine::GetInstance()->AddComponent(component);
    RendererManager::GetInstance()->pointLights.insert({number, component});
    RendererManager::GetInstance()->UpdateLight(id);
    return component;

}

std::shared_ptr<DirectionalLight> ComponentFactory::CreateDirectionalLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<DirectionalLight> component = parent->GetComponent<DirectionalLight>();
    if (component != nullptr) return component;
    id++;
    int number = 0;

    for (auto&& directionalLight : RendererManager::GetInstance()->directionalLights){
        if (directionalLight.second == nullptr) {
            component = std::make_shared<DirectionalLight>(parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            GloomEngine::GetInstance()->AddComponent(component);
            RendererManager::GetInstance()->directionalLights.find(number)->second = component;
            RendererManager::GetInstance()->UpdateLight(id);
            id++;
            return component;
        }
        number++;
    }

    component = std::make_shared<DirectionalLight>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    GloomEngine::GetInstance()->AddComponent(component);
    RendererManager::GetInstance()->directionalLights.insert({number, component});
    RendererManager::GetInstance()->UpdateLight(id);
    return component;
}

std::shared_ptr<SpotLight> ComponentFactory::CreateSpotLight(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<SpotLight> component = parent->GetComponent<SpotLight>();
    if (component != nullptr) return component;
    id++;
    int number = 0;

    for (auto&& spotLight : RendererManager::GetInstance()->spotLights){
        if (spotLight.second == nullptr) {
            component = std::make_shared<SpotLight>(parent, id);
            std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
            GloomEngine::GetInstance()->AddComponent(component);
            RendererManager::GetInstance()->spotLights.find(number)->second = component;
            RendererManager::GetInstance()->UpdateLight(id);
            return component;
        }
        number++;
    }
    component = std::make_shared<SpotLight>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    GloomEngine::GetInstance()->AddComponent(component);
    RendererManager::GetInstance()->spotLights.insert({number, component});
    RendererManager::GetInstance()->UpdateLight(id);
    return component;
}

std::shared_ptr<BoxCollider> ComponentFactory::CreateBoxCollider(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<BoxCollider> component = parent->GetComponent<BoxCollider>();
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<BoxCollider>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    ColliderManager::GetInstance()->boxColliders.insert({id, component});
    ColliderManager::GetInstance()->OnBoxColliderAdd();
    GloomEngine::GetInstance()->AddComponent(component);
    return component;
}

std::shared_ptr<Rigidbody> ComponentFactory::CreateRigidbody(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Rigidbody> component = parent->GetComponent<Rigidbody>();
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<Rigidbody>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    GloomEngine::GetInstance()->AddComponent(component);
    return component;
}

std::shared_ptr<Image> ComponentFactory::CreateImage(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<Image> component = parent->GetComponent<Image>();
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<Image>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    GloomEngine::GetInstance()->AddComponent(component);
    return component;
}

std::shared_ptr<PlayerMovement> ComponentFactory::CreatePlayerMovement(const std::shared_ptr<GameObject> &parent) {
    std::shared_ptr<PlayerMovement> component = parent->GetComponent<PlayerMovement>();
    if (component != nullptr) return component;
    id++;
    component = std::make_shared<PlayerMovement>(parent, id);
    std::shared_ptr<Component> parentComponent = std::dynamic_pointer_cast<Component>(component);
    GloomEngine::GetInstance()->AddComponent(component);
    return component;
}
