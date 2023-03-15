//
// Created by szymo on 01/03/2023.
//

#ifndef OPENGLGP_GAMEOBJECT_H
#define OPENGLGP_GAMEOBJECT_H

#include "include/ProjectSettings.h"
#include "include/Components/Transform.h"
#include "include/Factories/GameObjectFactory.h"
#include "include/Factories/ComponentFactory.h"

#include <memory>
#include <string>
#include <map>

class GloomEngine;
class Component;

class GameObject : public std::enable_shared_from_this<GameObject> {
private:
    // Name and id are unique
    int id;
    std::string name;

    std::map<int, std::shared_ptr<Component>> components;
    std::map<int, std::shared_ptr<GameObject>> children;

    static std::shared_ptr<GloomEngine> gloomEngine;
    static std::shared_ptr<GameObjectFactory> gameObjectFactory;
    static std::shared_ptr<ComponentFactory> componentFactory;

public:
    std::shared_ptr<GameObject> parent = nullptr;

    // Create file with tag names as enums
    Tags tag;

    std::shared_ptr<Transform> transform = nullptr;

    GameObject(const std::string &name, int id, const std::shared_ptr <GameObject> &parent = nullptr, Tags tag = Tags::DEFAULT);
    virtual ~GameObject();

    static void Init(const std::shared_ptr<GloomEngine> &gloomEngine);
    static std::shared_ptr<GameObject> Instantiate(std::string name, std::shared_ptr<GameObject> parent = nullptr, Tags tag = Tags::DEFAULT);
    static void Destroy(std::shared_ptr<GameObject> gameObject);

    template<class T>
    std::shared_ptr<T> AddComponent() {
        std::shared_ptr<T> component = std::dynamic_pointer_cast<T>(componentFactory->CreateComponent(typeid(T).name(), shared_from_this()));
        AddComponentToList(component);
        return component;
    };
    template<class T>
    std::shared_ptr<T> GetComponent() {
        for (auto&& component : components) {
            if (std::dynamic_pointer_cast<T>(component.second) != nullptr) {
                return std::dynamic_pointer_cast<T>(component.second);
            }
        }
        return nullptr;
    };
    std::shared_ptr<Component> GetComponentByName(ComponentNames name);

    void OnTransformUpdateComponents();
    void RemoveComponent(int componentId);
    void RemoveAllComponents();


    void SetParent(const std::shared_ptr<GameObject> &newParent);
    void AddChild(const std::shared_ptr<GameObject> &child);
    void RemoveChild(int childId);
    void RemoveAllChildren();


    void UpdateSelfAndChildren();
    void ForceUpdateSelfAndChildren();

    int GetId() const;
    const std::string &GetName() const;

private:
    void AddComponentToList(std::shared_ptr<Component> component);
};


#endif //OPENGLGP_GAMEOBJECT_H
