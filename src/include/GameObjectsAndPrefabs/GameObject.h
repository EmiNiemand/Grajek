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
struct AABB;

class GameObject : public std::enable_shared_from_this<GameObject> {
private:
    // Name and id are unique
    int id;
    std::string name;
    bool enabled = true;

public:
    std::map<int, std::shared_ptr<Component>> components;
    std::map<int, std::shared_ptr<GameObject>> children;

    std::shared_ptr<GameObject> parent = nullptr;
    std::shared_ptr<AABB> bounds = nullptr;

    Tags tag;

    glm::vec3 globalRotation = {0, 0, 0};
    std::shared_ptr<Transform> transform = nullptr;
    bool dirtyFlag = true;
    bool isOnFrustum = false;

public:
    GameObject(const std::string &name, int id, const std::shared_ptr <GameObject> &parent = nullptr, Tags tag = Tags::DEFAULT);
    virtual ~GameObject();

    static std::shared_ptr<GameObject> Instantiate(std::string name, std::shared_ptr<GameObject> parent = nullptr, Tags tag = Tags::DEFAULT);
    static void Destroy(std::shared_ptr<GameObject> gameObject);

    template<class T>
    std::shared_ptr<T> AddComponent() {
        std::shared_ptr<T> component = parent->GetComponent<T>();
        if (component != nullptr) return component;
        component = ComponentFactory::GetInstance()->CreateComponent<T>(shared_from_this());
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

    void EnableSelfAndChildren();
    void DisableSelfAndChildren();

    int GetId() const;
    const std::string &GetName() const;
    bool GetEnabled() const;

    void RecalculateGlobalRotation();
};


#endif //OPENGLGP_GAMEOBJECT_H
