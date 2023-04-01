#ifndef OPENGLGP_GAMEOBJECT_H
#define OPENGLGP_GAMEOBJECT_H

#include "ProjectSettings.h"
#include "Components/Transform.h"
#include "Factories/GameObjectFactory.h"
#include "Factories/ComponentFactory.h"
#include "Components/Component.h"

#include <memory>
#include <string>
#include <map>

class GloomEngine;

class GameObject : public std::enable_shared_from_this<GameObject> {
private:
    // Name and id are unique
    int id;
    std::string name;

public:
    std::map<int, std::shared_ptr<Component>> components;
    std::map<int, std::shared_ptr<GameObject>> children;

public:
    std::shared_ptr<GameObject> parent = nullptr;

    // Create file with tag names as enums
    Tags tag;

    std::shared_ptr<Transform> transform = nullptr;

    GameObject(const std::string &name, int id, const std::shared_ptr <GameObject> &parent = nullptr, Tags tag = Tags::DEFAULT);
    virtual ~GameObject();

    static std::shared_ptr<GameObject> Instantiate(std::string name, std::shared_ptr<GameObject> parent = nullptr, Tags tag = Tags::DEFAULT);
    static void Destroy(std::shared_ptr<GameObject> gameObject);

    template<class T>
    std::shared_ptr<T> AddComponent() {
        std::shared_ptr<T> component = parent->GetComponent<T>();
        if (component != nullptr) return component;
        component = ComponentFactory::GetInstance()->CreateComponent<T>(shared_from_this());
        component->OnCreate();
        components.insert({component->GetId(), component});
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
};


#endif //OPENGLGP_GAMEOBJECT_H
