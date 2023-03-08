//
// Created by szymo on 01/03/2023.
//

#ifndef OPENGLGP_GAMEOBJECT_H
#define OPENGLGP_GAMEOBJECT_H

#include "include/ProjectSettings.h"
#include "include/Components/Transform.h"
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

    std::shared_ptr<GloomEngine> gloomEngine;
    std::map<int,std::shared_ptr<Component>> components;
    std::map<int, std::shared_ptr<GameObject>> children;
public:
    std::shared_ptr<GameObject> parent = nullptr;

    // Create file with tag names as enums
    Tags tag;

    std::shared_ptr<Transform> transform = nullptr;

    GameObject(const std::shared_ptr <GloomEngine> &gloomEngine, const std::string &name, int id,
               const std::shared_ptr <GameObject> &parent = nullptr, Tags tag = Tags::DEFAULT);

    virtual ~GameObject();

    std::shared_ptr<Component> FindComponent(int componentId);
    std::shared_ptr<Component> FindComponentByName(ComponentNames name);
    void AddComponent(std::shared_ptr<Component> &component);
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
